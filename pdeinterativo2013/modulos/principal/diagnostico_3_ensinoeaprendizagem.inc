<?php
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "3.1. Planejamento Pedagógico", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_1_planejamentopedagogico"),
			  2 => array("id" => 3, "descricao" => "3.2. Tempo de Aprendizagem", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem"),
			  3 => array("id" => 4, "descricao" => "3.3. Síntese da dimensão 3", "link" => "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_3_sintesedimensao3")
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'diagnostico_3_0_orientacoes':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes";
		$pagAtiva = "diagnostico_3_0_orientacoes.inc";
		break;
	case 'diagnostico_3_1_planejamentopedagogico':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_1_planejamentopedagogico";
		$pagAtiva = "diagnostico_3_1_planejamentopedagogico.inc";
		break;
	case 'diagnostico_3_2_tempodeaprendizagem':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem";
		$pagAtiva = "diagnostico_3_2_tempodeaprendizagem.inc";
		break;
	case 'diagnostico_3_3_sintesedimensao3':
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_3_sintesedimensao3";
		$pagAtiva = "diagnostico_3_3_sintesedimensao3.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes";
		$pagAtiva = "diagnostico_3_0_orientacoes.inc";
		break;
}

echo "<br/>";
/*** Monta o segundo conjunto de abas ***/
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>