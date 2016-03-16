<?php
include_once "ajaxprimeirospassos.php";

// verificando se o browser é Konqueror
require APPRAIZ . "includes/classes/browser.class.inc"; 
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_KONQUEROR) {
	die("<script>
			alert('Atenção! Seu navegador de internet não é compatível com a plataforma SIMEC.".'\n'."Recomendamos a utilização dos seguintes navegadores:".'\n\n'."- Firefox".'\n'."- Google Chrome".'\n'."- Internet Explorer');
			window.location.href='pdeinterativo2013.php?modulo=principal/principal&acao=A';
		 </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';

echo '<script type="text/javascript" src="../pdeinterativo2013/js/pdeinterativo2013.js"></script>';
echo "<input id=\"hid_habil\" name=\"hid_habil\" type=\"hidden\" value=\"$habil\">";

echo "<br/>";

/*** Monta o primeiro conjunto de abas ***/
$db->cria_aba($abacod_tela,$url,$parametros);
barraProgressoPDEInterativo2013();
echo "<br/>";

/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=orientacoes"),
1 => array("id" => 2, "descricao" => "Passo 1", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso"),
2 => array("id" => 3, "descricao" => "Passo 2", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso"),
3 => array("id" => 4, "descricao" => "Passo 3", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso")
);

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba'] )
{
	case 'orientacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=orientacoes";
		$pagAtiva = "orientacoes.inc";
		break;
	case 'primeiropasso':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso";
		$pagAtiva = "primeiropasso.inc";
		break;
	case 'segundopasso':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso";
		$pagAtiva = "segundopasso.inc";
		break;
	case 'terceiropasso':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso";
		$pagAtiva = "terceiropasso.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A&aba=orientacoes";
		$pagAtiva = "orientacoes.inc";
		break;
}

/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;
verificaPermissao(PDEESC_PERFIL_DIRETOR);
?>