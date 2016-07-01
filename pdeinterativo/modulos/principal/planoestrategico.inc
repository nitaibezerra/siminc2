<?php 

if(CACHE_FILE) {

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

}

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
$_SESSION['pdeinterativo_vars']['usucpfdiretor']   = $arrDados['usucpf'];
$_SESSION['pdeinterativo_vars']['pdicodinep'] 	   = $arrDados['pdicodinep'];
$_SESSION['pdeinterativo_vars']['pdeid'] 		   = $arrDados['pdeid'];
$_SESSION['pdeinterativo_vars']['pdenome'] 		   = $arrDados['pdenome'];
$_SESSION['pdeinterativo_vars']['pditempdeescola'] = $arrDados['pditempdeescola'];

if(!$_SESSION['pdeinterativo_vars']['pdicodinep']){
	echo "<script>alert('Escola não encontrada!');window.location.href='pdeinterativo.php?modulo=principal/principal&acao=A'</script>";
	exit;
}

if($_SESSION['pdeinterativo_vars']['pditempdeescola']=="t") {

	$abrid = $db->pegaUm("SELECT abrid FROM pdeinterativo.abaresposta ab 
						  LEFT JOIN pdeinterativo.aba aa ON aa.abaid = ab.abaid  
						  WHERE aa.abacod='diagnostico_7_sintese' AND ab.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if(!$abrid) {
		echo "<script>alert('Para elaborar o plano de ação é necessário SALVAR a tela => 7. Síntese do Diagnóstico');window.location.href='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese'</script>";
		exit;
	}

} elseif($_SESSION['pdeinterativo_vars']['pditempdeescola']=="f") {


	if( !verificaRespostaAbaFormacao() ){
		echo "<script>alert('As abas obrigatórias são:".'\n\n'."- Primeiros Passos (1,2 e 3);".'\n'."- Dimensão 4 : Gestão -> Tela 4.1 Direção".'\n'."- Dimensão 5 : Comunidade escolar -> Tela 5.2 Docentes');window.location.href='pdeinterativo.php?modulo=principal/primeirospassos&acao=A';</script>";
		exit;
	}
	
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

include_once "_funcoesplanoestrategico_1.php";
include_once "_funcoesdiagnostico_6.php";
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
//$menu = array(
//			  0 => array("id" => 1, "descricao" => "1. PDE Escola", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola"),
//			  );

$menu = array(
			  0 => array("id" => 1, "descricao" => "1. PDE Escola", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola"),
			  1 => array("id" => 2, "descricao" => "2. Plano de Formação Continuada", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao")
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba'] )
{
	case 'planoestrategico_0_pdeescola':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola";
		$pagAtiva = "planoestrategico_0_pdeescola.inc";
		break;
	case 'formacao_0_planoformacao':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao";
		$pagAtiva = "planoformacao_0.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola";
		$pagAtiva = "planoestrategico_0_pdeescola.inc";
		break;
}

/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
switch( $_GET['aba'] )
{
	case 'planoestrategico_0_pdeescola':
		cabecalhoPDEInterativo($arrDados);
		break;
	case 'formacao_0_planoformacao':
		cabecalhoFormacao($arrDados);
		break;
	default:
		cabecalhoPDEInterativo($arrDados);
		break;
}
echo "<br>";
/*** Verifica qual aba está ativa ***/
include_once $pagAtiva;

if( $_GET['aba'] != 'formacao_0_planoformacao' ){
	verificaPermissao(PDEESC_PERFIL_DIRETOR);
}
?>