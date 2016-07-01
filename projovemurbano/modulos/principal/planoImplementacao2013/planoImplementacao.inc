<?php

if(!$_SESSION['projovemcampo']['apcid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemcampo.php?modulo=inicio&acao=C';</script>");

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$docid = $db->pegaUm("SELECT docid FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");

if(!$docid) {
	$docid = criaDocumento();
	$db->executar("UPDATE projovemcampo.adesaoprojovemcampo SET docid='".$docid."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
	$db->commit();
}

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuprojovemcampo(), "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A");


monta_titulo('Projovem Urbano', montaTituloEstMun());

switch( $_GET['aba'] )
{
	case 'metaMatriculaInicioAula':
		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula";
		$pagAtiva = "metaMatriculaInicioAula.inc";			
		break;
	case 'profissionais':
		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais";
		$pagAtiva = "profissionais.inc";			
		break;
// 	case 'formacaoEducadores':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores";
// 		$pagAtiva = "formacaoEducadores.inc";			
// 		break;
// 	case 'generoAlimenticios':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios";
// 		$pagAtiva = "generoAlimenticios.inc";			
// 		break;
// 	case 'qualificacaoProfissionalEstado':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissionalEstado";
// 		$pagAtiva = "qualificacaoProfissionalEstado.inc";			
// 		break;
// 	case 'qualificacaoProfissionalMunicipio':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissionalMunicipio";
// 		$pagAtiva = "qualificacaoProfissionalMunicipio.inc";			
// 		break;
// 	case 'transporteDidatico':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico";
// 		$pagAtiva = "transporteDidatico.inc";			
// 		break;
// 	case 'demaisAcoes':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes";
// 		$pagAtiva = "demaisAcoes.inc";			
// 		break;
// 	case 'resumoFinanceiro':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro";
// 		$pagAtiva = "resumoFinanceiro.inc";
// 		break;
// 	case 'repasseRecurso':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=repasseRecurso";
// 		$pagAtiva = "repasseRecurso.inc";
// 		break;
// 	case 'visualizarPlano':
// 		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=visualizarPlano";
// 		$pagAtiva = "visualizarPlano.inc";
// 		break;
	default:
		$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula";
		$pagAtiva = "metaMatriculaInicioAula";			
		break;
}

echo "<br>";

echo montarAbasArray(montaMenuPlanoImplementacao(), $abaAtiva);

include $pagAtiva;

?>