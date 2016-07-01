<?php
include APPRAIZ ."includes/workflow.php";

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

if($_REQUEST['unicod'] && $_REQUEST['unicod'] != $_SESSION['sisfor']['unicod']){
	$_SESSION['sisfor']['unicod'] = $_REQUEST['unicod'];
}

if($_REQUEST['curid'] && $_REQUEST['curid'] != $_SESSION['sisfor']['curid']){
	$_SESSION['sisfor']['curid'] = $_REQUEST['curid'];
}

if($_REQUEST['sifid'] && $_REQUEST['sifid'] != $_SESSION['sisfor']['sifid']) {
	
	$_SESSION['sisfor']['sifid'] = $_REQUEST['sifid'];
	
	unset($_SESSION['sisfor']['unicod'], $_SESSION['sisfor']['curid']);
	
} else {
	
	if($_REQUEST['curid'] &&  $_REQUEST['unicod']) {
	
		$_SESSION['sisfor']['ieoid'] = $db->pegaUm("SELECT ieoid FROM catalogocurso2014.iesofertante WHERE curid='".$_REQUEST['curid']."' AND unicod='".$_REQUEST['unicod']."' AND ieostatus='A'");
		
		if($_SESSION['sisfor']['ieoid']) 
			$_SESSION['sisfor']['sifid'] = $db->pegaUm("SELECT sifid FROM sisfor.sisfor WHERE ieoid='".$_SESSION['sisfor']['ieoid']."' AND unicod='".$_REQUEST['unicod']."' AND sifstatus='A'");
	
	}
	
}

if($_SESSION['sisfor']['sifid']) {
	
	$_SESSION['sisfor']['unicod'] = $db->pegaUm("SELECT unicod FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");

	if(!$_SESSION['sisfor']['curid']) $_SESSION['sisfor']['curid'] = $db->pegaUm("SELECT s.curid FROM sisfor.sisfor c
																				 INNER JOIN catalogocurso2014.iesofertante s ON s.ieoid = c.ieoid
																				 WHERE c.sifid='".$_SESSION['sisfor']['sifid']."'");
	
	
	if(!$_SESSION['sisfor']['curid']) $_SESSION['sisfor']['curid'] = $db->pegaUm("SELECT curid FROM sisfor.cursonaovinculado c
																				 INNER JOIN sisfor.sisfor s ON s.cnvid = c.cnvid
																				 WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");
	
	$_SESSION['sisfor']['docidprojeto'] = $db->pegaUm("SELECT docidprojeto FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
	
	if(!$_SESSION['sisfor']['docidprojeto']) {
		$_SESSION['sisfor']['docidprojeto'] = wf_cadastrarDocumento(WF_TPDID_PROJETO,"Projeto do curso {$_SESSION['sisfor']['curid']} e universidade {$_SESSION['sisfor']['unicod']}");
		$db->executar("UPDATE sisfor.sisfor SET docidprojeto='".$_SESSION['sisfor']['docidprojeto']."' WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
		$db->commit();
	}

    $_SESSION['sisfor']['docidcomposicaoequipe'] = $db->pegaUm("select docidcomposicaoequipe from sisfor.sisfor where sifid = '{$_SESSION['sisfor']['sifid']}'");

    if(!$_SESSION['sisfor']['docidcomposicaoequipe']){
        $_SESSION['sisfor']['docidcomposicaoequipe'] = wf_cadastrarDocumento(WF_TPDID_SISFOR_CADASTRAMENTO, "Cadastramento da equipe {$_SESSION['sisfor']['curid']} e universidade {$_SESSION['sisfor']['unicod']}");
        $db->executar("UPDATE sisfor.sisfor SET docidcomposicaoequipe ='".$_SESSION['sisfor']['docidcomposicaoequipe']."' WHERE sifid = '{$_SESSION['sisfor']['sifid']}'");
        $db->commit();
    }
    
    if(!$_SESSION['sisfor']['validado'][$_SESSION['sisfor']['sifid']]) {
    
    	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sisfor']['docidprojeto']."'");
    
    	if($esdid==ESD_PROJETO_VALIDADO) {
    		$_SESSION['sisfor']['validado'][$_SESSION['sisfor']['sifid']] = true;
    		$al = array("location"=>"sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A");
    		alertlocation($al);
    	}
    
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
$menu = array( 0 => array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=principalcurso"),
			   1 => array("id" => 2, "descricao" => "Dados Coordenador", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dadoscurso"),
			   2 => array("id" => 3, "descricao" => "Dados gerais do projeto", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dados_projeto"),
			   3 => array("id" => 4, "descricao" => "Estrutura da Formação", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=estrutura_curso"),
			   4 => array("id" => 5, "descricao" => "Equipe da IES", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=equipeies"),
			   5 => array("id" => 6, "descricao" => "Orçamento", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=orcamento"),
			   6 => array("id" => 7, "descricao" => "Visualização do Projeto", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=visualizacao_projeto"));

$estado = wf_pegarEstadoAtual( $_SESSION['sisfor']['docidprojeto'] );

if($estado['esdid'] == ESD_PROJETO_VALIDADO) {
	$menu[] = array("id" => 8, "descricao" => "Alterar Projeto", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=alterarprojeto");
}


echo "<br/>"; 
echo montarAbasArray($menu, $abaAtiva);  ?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script language="javascript" type="text/javascript" src="js/sisfor.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	<?php if(in_array(PFL_EQUIPE_MEC,$perfil)){ ?>

	<?php } ?>
</script>
<?  

if(is_file(APPRAIZ_SISFOR.'coordenador_curso/'.$aba.'.inc')) {
	include $aba.".inc";
} else {
	echo '<p align=center><b>Página não encontrada : '.$aba.'.inc</b></p>';
}

?>