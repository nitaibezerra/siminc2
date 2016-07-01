<?php

switch( $_GET['aba3'] )
{
	case 'resumoGeralEducadores':
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=profissionais&aba3=resumoGeralEducadores";
		$pagAtiva = "resumoGeralEducadores";
		break;
	case 'profissionaisResumo':
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=profissionais&aba3=profissionaisResumo";
		$pagAtiva = "profissionaisResumo";
		break;
	default:
		$abaAtiva = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=profissionais&aba3=profissionaisCadastro";
		$pagAtiva = "profissionaisCadastro";
		break;
}

echo "<br>";

$menu3[] = array("id" => 1, 
				"descricao" => "Profissionais", 
				"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=profissionais&aba3=profissionaisCadastro");
$menu3[] = array("id" => 2, 
				"descricao" => "Profissionais - Resumo", 
				"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=profissionais&aba3=profissionaisResumo");
$menu3[] = array("id" => 3, 
				"descricao" => "Resumo geral de educadores", 
				"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=profissionais&aba3=resumoGeralEducadores");


echo montarAbasArray($menu3, $abaAtiva);

include $pagAtiva.$_SESSION['projovemurbano']['ppuano'].'.inc';

?>
