<?php

switch( $_GET['aba3'] )
{
	case 'poloNucleoResumo':
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoResumo";
		$pagAtiva = "poloNucleoResumo";
		break;
	case 'poloNucleoGerenciar':
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar";
		$pagAtiva = "poloNucleoGerenciar";
		break;
	case 'poloNucleo':
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo";
		$pagAtiva = "poloNucleo";
		break;
	default:
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro";
		$pagAtiva = "poloNucleoCadastro";
		break;
}

echo "<br>";

$menu3[] = array("id" => 1, 
				"descricao" => "Pólo/Núcleo", 
				"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro");
if($_REQUEST['aba3']=="poloNucleoGerenciar") 
	$menu3[] = array("id" => 2, 
					"descricao" => 
					"Pólo/Núcleo - Gerenciar", 
					"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar");
$men3[] = array("id" => 3, 
				"descricao" => "Pólo/Núcleo - Resumo", 
				"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoResumo");


echo montarAbasArray($menu3, $abaAtiva);

include $pagAtiva.$_SESSION['projovemurbano']['ppuano'].'.inc';

?>
