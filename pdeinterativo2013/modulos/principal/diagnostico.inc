<?php

if(CACHE_FILE) {

	/* Início - Cache em arquivo*/
	
	$arrAbasCache[] = "diagnostico_1_1_ideb";
	$arrAbasCache[] = "diagnostico_1_2_taxasderendimento";
	$arrAbasCache[] = "diagnostico_1_3_provabrasil";
	$arrAbasCache[] = "diagnostico_2_1_matriculas";
	$arrAbasCache[] = "diagnostico_2_2_distorcaoidadeserie";
	$arrAbasCache[] = "diagnostico_2_3_aproveitamentoescolar";
	$arrAbasCache[] = "diagnostico_2_4_areasdeconhecimento";
	$arrAbasCache[] = "diagnostico_5_2_docentes";
	
	if($_GET['aba1'] && !$_POST && in_array($_GET['aba1'],$arrAbasCache)){
		
		include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
		// Cacheamento por perfil
		$perfis = pegaPerfilGeral();
		if(in_array(PDEINT_PERFIL_EQUIPE_FNDE,$perfis)) $pfl = PDEINT_PERFIL_EQUIPE_FNDE;
		if(in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL,$perfis)) $pfl = PDEINT_PERFIL_CONSULTA_ESTADUAL;
		if(in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL,$perfis)) $pfl = PDEINT_PERFIL_CONSULTA_MUNICIPAL;
		if(in_array(PDEESC_PERFIL_CONSULTA,$perfis)) $pfl = PDEESC_PERFIL_CONSULTA;
		if(in_array(PDEESC_PERFIL_DIRETOR,$perfis)) $pfl = PDEESC_PERFIL_DIRETOR;
		if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$perfis)) $pfl = PDEINT_PERFIL_COMITE_ESTADUAL;
		if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$perfis)) $pfl = PDEINT_PERFIL_COMITE_MUNICIPAL;
		if(in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$perfis)) $pfl = PDEINT_PERFIL_COMITE_PAR_ESTADUAL;
		if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$perfis)) $pfl = PDEINT_PERFIL_COMITE_PAR_MUNICIPAL;
		if(in_array(PDEINT_PERFIL_EQUIPE_MEC,$perfis)) $pfl = PDEINT_PERFIL_EQUIPE_MEC;
		if($db->testa_superuser()) $pfl = PDEINT_PERFIL_SUPER_USUARIO;
		if(!$pfl) $pfl="semperfil";
		
		$cache = new cache($_GET['aba1']."_".$_SESSION['pdeinterativo2013_vars']['pdeid']."_".$pfl);
		
	}
	/* Fim - Cache em arquivo*/
	
	/* Início - Cache para Requisições*/
	
	$arrRequisicao[] = "montaGraficoTaxasRendimento";
	$arrRequisicao[] = "montaGraficoIDEB";
	$arrRequisicao[] = "montaGraficoProvaBrasil";
	$arrRequisicao[] = "montaGraficoDistorcao";
	$arrRequisicao[] = "montaGraficoAproveitamentoEstudantes";
	
	if($_REQUEST['requisicao'] && !$_POST && in_array($_REQUEST['requisicao'],$arrRequisicao)){
		
		include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
		$arrParametros = $_GET;
		unset($arrParametros['modulo']);
		unset($arrParametros['acao']);
		unset($arrParametros['requisicao']);
		if($arrParametros){
			foreach($arrParametros as $chave => $valor){
				$arrNome[] = "$chave=$valor";
			}
			$chave = implode(",",$arrNome);
			$nome = $_REQUEST['requisicao']."_".$chave;
		}else{
			$nome = $_REQUEST['requisicao'];
		}
		
		$cache = new cache($nome."_".$_SESSION['pdeinterativo2013_vars']['pdeid'],"png");
	}
	/* Fim - Cache para Requisições*/

}

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo2013_vars']['usucpfdiretor']);
$_SESSION['pdeinterativo2013_vars']['usucpfdiretor'] = $arrDados['usucpf'];
$_SESSION['pdeinterativo2013_vars']['pdicodinep'] = $arrDados['pdicodinep'];
$_SESSION['pdeinterativo2013_vars']['pdeid'] = $arrDados['pdeid'];
$_SESSION['pdeinterativo2013_vars']['pdenome'] = $arrDados['pdenome'];

if(!$_SESSION['pdeinterativo2013_vars']['pdicodinep']){
	echo "<script>alert('Escola não encontrada!');window.location.href='pdeinterativo2013.php?modulo=principal/principal&acao=A'</script>";
	exit;
}

// verificando se o browser é Konqueror
require APPRAIZ . "includes/classes/browser.class.inc"; 
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_KONQUEROR) {
	die("<script>
			alert('Atenção! Seu navegador de internet não é compatível com a plataforma SIMEC.".'\n'."Recomendamos a utilização dos seguintes navegadores:".'\n\n'."- Firefox".'\n'."- Google Chrome".'\n'."- Internet Explorer');
			window.location.href='pdeinterativo2013.php?modulo=principal/principal&acao=A';
		 </script>");
}

include_once "_funcoesdiagnostico_1.php";
include_once "_funcoesdiagnostico_2.php";
include_once "_funcoesdiagnostico_3.php";
include_once "_funcoesdiagnostico_4.php";
include_once "_funcoesdiagnostico_5.php";
include_once "_funcoesdiagnostico_6.php";
include_once "_funcoesdiagnostico_7.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";
echo '<link rel="stylesheet" type="text/css" href="../pdeinterativo2013/css/pdeinterativo2013.css"/>';
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script type="text/javascript" src="../pdeinterativo2013/js/pdeinterativo2013.js"></script>';

/*** Monta o primeiro conjunto de abas ***/
$db->cria_aba($abacod_tela,$url,$parametros);
barraProgressoPDEInterativo2013();
echo "<br/>";


/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_0_orientacoes"),
			1 => array("id" => 2, "descricao" => "1. Indicadores e taxas", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas"),  
			2 => array("id" => 3, "descricao" => "2. Distorção e aproveitamento", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento"),
			3 => array("id" => 4, "descricao" => "3. Ensino e aprendizagem", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem"),
			4 => array("id" => 5, "descricao" => "4. Gestão", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao"),
			5 => array("id" => 6, "descricao" => "5. Comunidade escolar", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar"),
			6 => array("id" => 7, "descricao" => "6. Infraestrutura", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura"),
			7 => array("id" => 8, "descricao" => "7. Síntese do diagnóstico", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese")
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba'] )
{
	case 'orientacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=orientacoes";
		$pagAtiva = "orientacoes.inc";
		break;
	case 'diagnostico_1_indicadoresetaxas':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas";
		$pagAtiva = "diagnostico_1_indicadoresetaxas.inc";
		break;
	case 'diagnostico_2_distorcaoeaproveitamento':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento";
		$pagAtiva = "diagnostico_2_distorcaoeaproveitamento.inc";
		break;
	case 'diagnostico_3_ensinoeaprendizagem':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem";
		$pagAtiva = "diagnostico_3_ensinoeaprendizagem.inc";
		break;
	case 'diagnostico_4_gestao':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao";
		$pagAtiva = "diagnostico_4_gestao.inc";
		break;
	case 'diagnostico_5_comunidadeescolar':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar";
		$pagAtiva = "diagnostico_5_comunidadeescolar.inc";
		break;
	case 'diagnostico_6_infraestrutura':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura";
		$pagAtiva = "diagnostico_6_infraestrutura.inc";
		break;
	case 'diagnostico_7_sintese':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese";
		$pagAtiva = "diagnostico_7_sintese.inc";
		break;
	case 'diagnostico_8_conclusao':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_8_conclusao";
		$pagAtiva = "diagnostico_8_conclusao.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_0_orientacoes";
		$pagAtiva = "diagnostico_0_orientacoes.inc";
		break;
}

/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
cabecalhoPDEInterativo2013($arrDados);
include_once $pagAtiva;

verificaPermissao(PDEESC_PERFIL_DIRETOR);
?>