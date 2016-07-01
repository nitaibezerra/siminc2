<?php
include_once APPRAIZ."/www/projovemcampo/_funcoes_planoImplementacao.php";

if(!$_SESSION['projovemcampo']['apcid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemcampo.php?modulo=inicio&acao=C';</script>");

criaPlanodeImplementacao();



$desbloqueia = liberademaisacoes();

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
$docid = $db->pegaUm("SELECT docid FROM projovemcampo.planodeimplementacao WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");

if(!$docid) {
	$docid = criaDocumento();
	$db->executar("UPDATE projovemcampo.planodeimplementacao SET docid='".$docid."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
	$db->commit();
}

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuprojovemcampo(), "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A");


monta_titulo('Projovem Campo', montaTituloEstMun());
switch( $_GET['aba'] )
{
	case 'metaMatriculaInicioAula':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula";
		$pagAtiva = "metaMatriculaInicioAula";			
		break;
	case 'profissionais':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais";
		$pagAtiva = "profissionais";			
		break;
	case 'formacaoEducadores':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores";
		$pagAtiva = "formacaoEducadores";			
		break;
	case 'generoAlimenticios':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios";
		$pagAtiva = "generoAlimenticios";			
		break;
	case 'qualificacaoProfissional':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissional";
		$pagAtiva = "qualificacaoProfissional";			
		break;
	case 'transporteDidatico':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico";
		$pagAtiva = "transporteDidatico";			
		break;
	case 'demaisAcoes':
	if($desbloqueia == 't'){
			$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes";
			$pagAtiva = "demaisAcoes";			
			break;
	}
	case 'enderecoEntrega':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=enderecoEntrega";
		$pagAtiva = "enderecoEntrega";
		break;
	case 'resumoFinanceiro':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro";
		$pagAtiva = "resumoFinanceiro";
		break;
	case 'repasseRecurso':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=repasseRecurso";
		$pagAtiva = "repasseRecurso";
		break;
	case 'visualizarPlano':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=visualizarPlano";
		$pagAtiva = "visualizarPlano";
		break;
	case 'tramitaPlanos':
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=tramitaPlanos";
		$pagAtiva = "tramitaPlanos";
		break;
	default:
		$abaAtiva = "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula";
		$pagAtiva = "metaMatriculaInicioAula";			
		break;
}

echo "<br>";

	$menu[] = array("id" => 1, 
					"descricao" => "Meta, Matrícula e Início de aula", 
					"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula");
	$menu[] = array("id" => 2, 
					"descricao" => "Profissionais", 
					"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais");
	$menu[] = array("id" => 3, 
					"descricao" => "Formação de Educadores, Formadores e Gestores", 
					"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores");
 	$menu[] = array("id" => 4, 
                    "descricao" => "Gêneros Alimenticios", 
                    "link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios");
 	$menu[] = array("id" => 5, "descricao" => 
                    "Qualificação Profissional", 
                    "link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissional");
// 	if(!$_SESSION['projovemcampo']['muncod']){
		$menu[] = array("id" => 6,
					"descricao" => "Transporte Mat. Didático", 
					"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico");
// 	}
	if($desbloqueia){
	 	$menu[] = array("id" => 7,
	                    "descricao" => "Demais Ações", 
	                    "link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes");
	}
// 	if(!$_SESSION['projovemcampo']['muncod']){
		$menu[] = array("id" => 8,
				"descricao" => "Endereço Entrega Mat. Didático",
				"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=enderecoEntrega");
// 	}
//   	$menu[] = array("id" => 9, 
//                      "descricao" => "Resumo Financeiro", 
//                      "link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro");
 	$menu[] = array("id" => 10, 
                    "descricao" => "Repasse de Recurso", 
                    "link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=repasseRecurso");
 	$menu[] = array("id" => 11, 
                    "descricao" => "Visualizar Plano", 
                    "link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=visualizarPlano");
 	$menu[] = array("id" => 12,
		 			"descricao" => "Tramitação - Plano de Implementação",
		 			"link" => "projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=tramitaPlanos");
echo montarAbasArray($menu, $abaAtiva);
monta_titulo(' ', ''); //212
// ver($pagAtiva,d);
include 'planoimplementacao/'.$pagAtiva.'.inc';

?>