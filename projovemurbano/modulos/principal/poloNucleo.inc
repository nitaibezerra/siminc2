<?php

switch( $_GET['aba2'] )
{
	case 'poloNucleoResumo':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoResumo";
		$pagAtiva = "poloNucleoResumo.inc";
		break;
	case 'poloNucleoGerenciar':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoGerenciar";
		$pagAtiva = "poloNucleoGerenciar.inc";
		break;
	case 'poloNucleo':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo";
		$pagAtiva = "poloNucleo.inc";
		break;
	default:
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro";
		$pagAtiva = "poloNucleoCadastro.inc";
		break;
}

echo "<br>";

echo montarAbasArray(montaMenuPoloNucleo(), $abaAtiva);

include $pagAtiva;

?>
