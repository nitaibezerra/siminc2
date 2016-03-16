<?php
switch( $_GET['aba2'] )
{
	case 'resumoGeralEducadores':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=resumoGeralEducadores";
		$pagAtiva = "resumoGeralEducadores";
		break;
	case 'profissionaisResumo':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisResumo";
		$pagAtiva = "profissionaisResumo";
		break;
	default:
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisCadastro";
		$pagAtiva = "profissionaisCadastro";
		break;
}

echo "<br>";

$menu2[] = array("id" => 1, 
				"descricao" => "Profissionais", 
				"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisCadastro");
$menu2[] = array("id" => 2, 
				"descricao" => "Profissionais - Resumo", 
				"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisResumo");
$menu2[] = array("id" => 3, 
				"descricao" => "Resumo geral de educadores", 
				"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=resumoGeralEducadores");

echo montarAbasArray($menu2, $abaAtiva);
if (!empty($pagAtiva)) {
	
	include $pagAtiva.'.inc';
}
?>
