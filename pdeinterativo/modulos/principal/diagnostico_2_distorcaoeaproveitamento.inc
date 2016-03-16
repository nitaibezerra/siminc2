<?php
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "2.1. Matrícula", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_1_matriculas"),
			  2 => array("id" => 3, "descricao" => "2.2. Distorção idade-série", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_2_distorcaoidadeserie"),
			  3 => array("id" => 4, "descricao" => "2.3. Aproveitamento escolar", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_3_aproveitamentoescolar"),
			  4 => array("id" => 5, "descricao" => "2.4. Áreas de conhecimento", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento"),
			  5 => array("id" => 6, "descricao" => "2.5. Síntese da Dimensão 2", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_5_sintesedimensao2")
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'diagnostico_2_0_orientacoes':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_0_orientacoes";
		$pagAtiva = "diagnostico_2_0_orientacoes.inc";
		break;
	case 'diagnostico_2_1_matriculas':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_1_matriculas";
		$pagAtiva = "diagnostico_2_1_matriculas.inc";
		break;
	case 'diagnostico_2_2_distorcaoidadeserie':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_2_distorcaoidadeserie";
		$pagAtiva = "diagnostico_2_2_distorcaoidadeserie.inc";
		break;
	case 'diagnostico_2_3_aproveitamentoescolar':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_3_aproveitamentoescolar";
		$pagAtiva = "diagnostico_2_3_aproveitamentoescolar.inc";
		break;
	case 'diagnostico_2_4_areasdeconhecimento':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento";
		$pagAtiva = "diagnostico_2_4_areasdeconhecimento.inc";
		break;
	case 'diagnostico_2_5_sintesedimensao2':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_5_sintesedimensao2";
		$pagAtiva = "diagnostico_2_5_sintesedimensao2.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_0_orientacoes";
		$pagAtiva = "diagnostico_2_0_orientacoes.inc";
		break;
}

echo "<br/>";
/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>