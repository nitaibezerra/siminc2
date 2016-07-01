<?php

switch( $_GET['aba3'] )
{
	case 'poloNucleoResumo':
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoResumo2014";
		$pagAtiva = "poloNucleoResumo";
		break;
	case 'poloNucleoGerenciar':
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar2014";
		$pagAtiva = "poloNucleoGerenciar";
		break;
	case 'poloNucleo':
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo2014";
		$pagAtiva = "poloNucleo";
		break;
	default:
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro2014";
		$pagAtiva = "poloNucleoCadastro";
		break;
}

echo "<br>";

$menu3[] = array("id" => 1, 
				"descricao" => "Pólo/Núcleo", 
				"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro2014");
if($_REQUEST['aba3']=="poloNucleoGerenciar") 
	$menu3[] = array("id" => 2, 
					"descricao" => 
					"Pólo/Núcleo - Gerenciar", 
					"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar2014");
$men3[] = array("id" => 3, 
				"descricao" => "Pólo/Núcleo - Resumo", 
				"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoResumo2014");


echo montarAbasArray($menu3, $abaAtiva);

include $pagAtiva.$_SESSION['projovemurbano']['ppuano'].'.inc';

?>
