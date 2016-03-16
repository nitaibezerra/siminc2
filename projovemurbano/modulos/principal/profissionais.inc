<?php

switch( $_GET['aba2'] )
{
	case 'ss':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel";
		$pagAtiva = "coordenadorResponsavel.inc";
		break;
	case 'resumoGeralEducadores':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=resumoGeralEducadores";
		$pagAtiva = "resumoGeralEducadores.inc";
		break;
	case 'profissionaisResumo':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisResumo";
		$pagAtiva = "profissionaisResumo.inc";
		break;
	default:
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisCadastro";
		$pagAtiva = "profissionaisCadastro.inc";
		break;
}

echo "<br>";

echo montarAbasArray(montaMenuProfissionais(), $abaAtiva);

include $pagAtiva;

?>
