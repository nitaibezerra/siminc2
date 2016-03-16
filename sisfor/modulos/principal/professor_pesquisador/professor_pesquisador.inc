<?php
include APPRAIZ ."includes/workflow.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit();
}


require_once APPRAIZ . "includes/cabecalho.inc";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

$_SESSION['sisfor']['iusd'] = $db->pegaUm("SELECT i.iusd FROM sisfor.identificacaousuario i INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd WHERE i.iuscpf='".$_SESSION['usucpf']."' AND t.pflcod='".PFL_PROFESSOR_PESQUISADOR."'");

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

//montaCabecalhoCoordenadorCurso();

$perfil = pegarPerfil($_SESSION['usucpf']);

$aba = !$_GET['aba'] ? "principalcurso" : $_GET['aba'];

$abaAtiva = "sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A&aba=$aba";

/*** Array com os itens da aba de identificação ***/
$menu = array( 0 => array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A&aba=principalcurso"),
			   1 => array("id" => 2, "descricao" => "Dados Professor pesquisador", "link" => "sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A&aba=dadoscurso"),
			   2 => array("id" => 3, "descricao" => "Avaliar cursistas", "link" => "sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A&aba=cadastrar_cursista"),
			   3 => array("id" => 4, "descricao" => "Acompanhar bolsas", "link" => "sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A&aba=acompanhamentoavaliacaobolsas")
			 //  3 => array("id" => 4, "descricao" => "Estrutura da Formação", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=estrutura_curso"),
			 //  4 => array("id" => 5, "descricao" => "Equipe da IES", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=equipeies"),
			 //  5 => array("id" => 6, "descricao" => "Orçamento", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=orcamento"),
			 //  6 => array("id" => 7, "descricao" => "Visualização do Projeto", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=visualizacao_projeto")
                           );

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
<?php  include_once $aba.".inc"; ?>