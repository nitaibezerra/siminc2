<?php
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "5.1. Estudantes", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_1_estudantes"),
			  2 => array("id" => 3, "descricao" => "5.2. Docentes", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes"),
			  3 => array("id" => 4, "descricao" => "5.3. Demais profissionais", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais"),
			  4 => array("id" => 5, "descricao" => "5.4. Pais e comunidade", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade"),
			  5 => array("id" => 6, "descricao" => "5.5. Síntese da Dimensão 5", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_5_sintesedimensao5")
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'diagnostico_5_0_orientacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_0_orientacoes";
		$pagAtiva = "diagnostico_5_0_orientacoes.inc";
		break;
	case 'diagnostico_5_1_estudantes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_1_estudantes";
		$pagAtiva = "diagnostico_5_1_estudantes.inc";
		break;
	case 'diagnostico_5_2_docentes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes";
		$pagAtiva = "diagnostico_5_2_docentes.inc";
		break;
	case 'diagnostico_5_3_demaisprofissionais':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais";
		$pagAtiva = "diagnostico_5_3_demaisprofissionais.inc";
		break;
	case 'diagnostico_5_4_paisecomunidade':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade";
		$pagAtiva = "diagnostico_5_4_paisecomunidade.inc";
		break;
	case 'diagnostico_5_5_sintesedimensao5':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_5_sintesedimensao5";
		$pagAtiva = "diagnostico_5_5_sintesedimensao5.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_0_orientacoes";
		$pagAtiva = "diagnostico_5_0_orientacoes.inc";
		break;
}

echo "<br/>";
/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>