<?php
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "1.1. IDEB", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_1_ideb"),
			  2 => array("id" => 3, "descricao" => "1.2. Taxas de rendimento", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento"),
			  3 => array("id" => 4, "descricao" => "1.3. Prova Brasil", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil"),
			  4 => array("id" => 5, "descricao" => "1.4. Síntese da Dimensão 1", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_4_sintesedimensao1")
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'diagnostico_1_0_orientacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_0_orientacoes";
		$pagAtiva = "diagnostico_1_0_orientacoes.inc";
		break;
	case 'diagnostico_1_1_ideb':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_1_ideb";
		$pagAtiva = "diagnostico_1_1_ideb.inc";
		break;
	case 'diagnostico_1_2_taxasderendimento':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento";
		$pagAtiva = "diagnostico_1_2_taxasderendimento.inc";
		break;
	case 'diagnostico_1_3_provabrasil':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil";
		$pagAtiva = "diagnostico_1_3_provabrasil.inc";
		break;
	case 'diagnostico_1_4_sintesedimensao1':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_4_sintesedimensao1";
		$pagAtiva = "diagnostico_1_4_sintesedimensao1.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_0_orientacoes";
		$pagAtiva = "diagnostico_1_0_orientacoes.inc";
		break;
}

echo "<br/>";
/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>