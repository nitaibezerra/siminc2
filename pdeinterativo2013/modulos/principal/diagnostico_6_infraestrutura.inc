<?php
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "6.1. Instalações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_1_instalacoes"),
			  2 => array("id" => 3, "descricao" => "6.2. Equipamentos", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_2_equipamentos"),
			  3 => array("id" => 4, "descricao" => "6.3. Síntese da Dimensão 6", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese"),
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'diagnostico_6_1_instalacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_1_instalacoes";
		$pagAtiva = "diagnostico_6_1_instalacoes.inc";
		break;
	case 'diagnostico_6_2_equipamentos':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_2_equipamentos";
		$pagAtiva = "diagnostico_6_2_equipamentos.inc";
		break;
	case 'diagnostico_6_3_sintese':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese";
		$pagAtiva = "diagnostico_6_3_sintese.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_0_orientacoes";
		$pagAtiva = "diagnostico_6_0_orientacoes.inc";
		break;
}

echo "<br/>";
/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>