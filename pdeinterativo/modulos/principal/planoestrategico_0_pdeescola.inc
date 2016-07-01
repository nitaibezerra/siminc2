<?php 

/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
			  0 => array("id" => 1, "descricao" => "Orientações", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_0_orientacoes"),
			  1 => array("id" => 2, "descricao" => "1.1 Grandes Desafios", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios"),
			  2 => array("id" => 3, "descricao" => "1.2 Planos de ação", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao"),
			  3 => array("id" => 4, "descricao" => "1.3 Visualizar PDE", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_3_visualizarplanoacao")
			  
			  );

/*** Verifica qual aba está ativa ***/
switch( $_GET['aba1'] )
{
	case 'planoestrategico_0_0_orientacoes':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_0_orientacoes";
		$pagAtiva = "planoestrategico_0_0_orientacoes.inc";
		break;
	case 'planoestrategico_0_1_grandesdesafios':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios";
		$pagAtiva = "planoestrategico_0_1_grandesdesafios.inc";
		break;
	case 'planoestrategico_0_2_planoacao':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao";
		$pagAtiva = "planoestrategico_0_2_planoacao.inc";
		break;
	case 'planoestrategico_0_3_visualizarplanoacao':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_3_visualizarplanoacao";
		$pagAtiva = "planoestrategico_0_3_visualizarplanoacao.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_0_orientacoes";
		$pagAtiva = "planoestrategico_0_0_orientacoes.inc";
		break;
}

/*** Monta o segundo conjunto de abas ***/
echo "<br/>";
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>