<?php 
/* Início - Cache em arquivo*/
$arrAbasCache[] = "planoestrategico_0_3_visualizarplanoacao";
if($_GET['aba1'] && !$_POST && in_array($_GET['aba1'],$arrAbasCache)){
	include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
	if($_SESSION['pdeinterativo_vars']['pdeid']) {
		// Cacheamento por perfil
		$perfis = pegaPerfilGeral();
		if(in_array(PDEINT_PERFIL_EQUIPE_FNDE,$perfis)) $pfl = PDEINT_PERFIL_EQUIPE_FNDE;
		if(in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL,$perfis)) $pfl = PDEINT_PERFIL_CONSULTA_ESTADUAL;
		if(in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL,$perfis)) $pfl = PDEINT_PERFIL_CONSULTA_MUNICIPAL;
		if(in_array(PDEESC_PERFIL_CONSULTA,$perfis)) $pfl = PDEESC_PERFIL_CONSULTA;
		if(in_array(PDEESC_PERFIL_DIRETOR,$perfis)) $pfl = PDEESC_PERFIL_DIRETOR;
		if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$perfis)) $pfl = PDEINT_PERFIL_COMITE_ESTADUAL;
		if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$perfis)) $pfl = PDEINT_PERFIL_COMITE_MUNICIPAL;
		if(in_array(PDEINT_PERFIL_EQUIPE_MEC,$perfis)) $pfl = PDEINT_PERFIL_EQUIPE_MEC;
		if($db->testa_superuser()) $pfl = PDEINT_PERFIL_SUPER_USUARIO;
		$cache = new cache($_GET['aba1']."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".(($pfl)?$pfl:'semperfil'));
	}
}
/* Fim - Cache em arquivo*/

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
$_SESSION['pdeinterativo_vars']['usucpfdiretor'] = $arrDados['usucpf'];
$_SESSION['pdeinterativo_vars']['pdicodinep'] = $arrDados['pdicodinep'];
$_SESSION['pdeinterativo_vars']['pdeid'] = $arrDados['pdeid'];
$_SESSION['pdeinterativo_vars']['pdenome'] = $arrDados['pdenome'];

if(!$_SESSION['pdeinterativo_vars']['pdicodinep']){
	echo "<script>alert('Escola não encontrada!');window.location.href='pdeinterativo.php?modulo=principal/principal&acao=A'</script>";
	exit;
}

$abrid = $db->pegaUm("SELECT abrid FROM pdeinterativo.abaresposta ab 
					  LEFT JOIN pdeinterativo.aba aa ON aa.abaid = ab.abaid  
					  WHERE aa.abacod='diagnostico_7_sintese' AND ab.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");

if(!$abrid) {
	echo "<script>alert('Para elaborar o plano de ação é necessário SALVAR a tela => 7. Síntese do Diagnóstico');window.location.href='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese'</script>";
	exit;
}

// verificando se o browser é Konqueror
require APPRAIZ . "includes/classes/browser.class.inc"; 
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_KONQUEROR) {
	die("<script>
			alert('Atenção! Seu navegador de internet não é compatível com a plataforma SIMEC.".'\n'."Recomendamos a utilização dos seguintes navegadores:".'\n\n'."- Firefox".'\n'."- Google Chrome".'\n'."- Internet Explorer');
			window.location.href='pdeinterativo.php?modulo=principal/principal&acao=A';
		 </script>");
}

include_once "_funcoes_formacao.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";
echo '<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>';
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>';

/*** Monta o primeiro conjunto de abas ***/
$db->cria_aba($abacod_tela,$url,$parametros);
barraProgressoPDEInterativo();
echo "<br/>";


/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "1. Plano de Formação", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/formacao&acao=A&aba=formacao_0_planoformacao")
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba'] )
{
	case 'formacao_0_planoformacao':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/formacao&acao=A&aba=formacao_0_planoformacao";
		$pagAtiva = "formacao_0_planoformacao.inc";
		break;
	case 'formacao_1_cursosugerido':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/formacao&acao=A&aba=formacao_0_planoformacao";
		$pagAtiva = "formacao_1_cursosugerido.inc";
		break;
	case 'formacao_2_relacaoprioridade':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/formacao&acao=A&aba=formacao_0_planoformacao";
		$pagAtiva = "formacao_2_relacaoprioridade.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/formacao&acao=A&aba=formacao_0_planoformacao";
		$pagAtiva = "formacao_0_planoformacao.inc";
		break;
}

/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);
echo "<br/>";

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

verificaPermissao(PDEESC_PERFIL_DIRETOR);
?>