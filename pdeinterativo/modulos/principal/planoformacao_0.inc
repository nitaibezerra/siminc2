<?php
// Alteração requerida por wallace 31/05/2012 as 18:05 -> in_array(PDEESC_PERFIL_DIRETOR,$arrPerfil) )
// Nova alteração requerida por Wallace 11/06/2012 as 14:13 -> if( !$db->testa_superuser() && $esfera != 'FEDERAL' ){
// Nova alteração requerida por Wallace 23/07/2012 liberar para todos usuarios com permissão
$esfera = pegaEsfera();
$arrPerfil = pegaPerfilGeral();

//if( !$db->testa_superuser() && $esfera != 'FEDERAL' ){
//if( in_array(PDEESC_PERFIL_DIRETOR,$arrPerfil) ){
//	echo "<script>
//			alert('O prazo para envio do Plano de Formação Continuada encerrou no dia 30 de abril de 2012. Se a sua escola não encaminhou-o para a Secretaria até aquela data, poderá fazê-lo futuramente. O PDE Interativo continuará aberto apenas para a elaboração do Diagnóstico e dos Planos de Ação..');
//			window.location = 'pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola';
//		</script>";
//	exit;
//}
//Fim alteração
/*** Array com os itens da aba de primeiros passos ***/
$menu = array(
  0 => array("id" => 1, 
  			 "descricao" => "Orientações", 
  			 "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_orientacoes"),
  1 => array("id" => 2, 
  			 "descricao" => "2.1 Proposta da Escola", 
  			 "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_planoformacao"),
  2 => array("id" => 3, 
  			 "descricao" => "2.2 Ordem de Prioridade", 
  			 "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_2_relacaoprioridade"),
  3 => array("id" => 4, 
  			 "descricao" => "2.3 Demanda Social", 
  			 "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social"),
  4 => array("id" => 5, 
  			 "descricao" => "2.4 Sugestão de Curso", 
  			 "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_1_cursosugerido"),
  5 => array("id" => 6, 
  			 "descricao" => "2.5 Visualização do Plano de Formação Continuada", 
  			 "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_4_vizualizacao")
  );
  
switch( $_GET['aba1'] )
{
	
	case 'formacao_0_orientacoes':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_orientacoes";
		$pagAtiva = "formacao_0_orientacoes.inc";
		break;
	case 'formacao_0_planoformacao':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_planoformacao";
		$pagAtiva = "formacao_0_planoformacao.inc";
		break;
	case 'formacao_1_cursosugerido':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_1_cursosugerido";
		$pagAtiva = "formacao_1_cursosugerido.inc";
		break;
	case 'formacao_2_relacaoprioridade':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_2_relacaoprioridade";
		$pagAtiva = "formacao_2_relacaoprioridade.inc";
		break;
	case 'formacao_3_demanda_social':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social";
		$pagAtiva = "formacao_3_demanda_social.inc";
		break;
	case 'formacao_4_vizualizacao':
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_4_vizualizacao";
		$pagAtiva = "formacao_4_vizualizacao.inc";
		break;
	default:
		$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_orientacoes";
		$pagAtiva = "formacao_0_orientacoes.inc";
		break;
}

/*** Monta o segundo conjunto de abas ***/
echo "<br/>";
echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once $pagAtiva;

?>