<?php
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "4.1. Direção", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_1_direcao"),
			  2 => array("id" => 3, "descricao" => "4.2. Processos", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos"),
			  3 => array("id" => 4, "descricao" => "4.3. Finanças", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas"),
			  4 => array("id" => 5, "descricao" => "4.4. Síntese da Dimensão 4", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_4_sintesedimensao4")
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'diagnostico_4_0_orientacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes";
		$pagAtiva = "diagnostico_4_0_orientacoes.inc";
		break;
	case 'diagnostico_4_1_direcao':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_1_direcao";
		$pagAtiva = "diagnostico_4_1_direcao.inc";
		break;
	case 'diagnostico_4_2_processos':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos";
		$pagAtiva = "diagnostico_4_2_processos.inc";
		break;
	case 'diagnostico_4_3_financas':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas";
		$pagAtiva = "diagnostico_4_3_financas.inc";
		break;
	case 'diagnostico_4_4_sintesedimensao4':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_4_sintesedimensao4";
		$pagAtiva = "diagnostico_4_4_sintesedimensao4.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes";
		$pagAtiva = "diagnostico_4_0_orientacoes.inc";
		break;
}

echo "<br/>";
/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>