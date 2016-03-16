<?php

if(!$_SESSION['projovemurbano']['pjuid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemurbano.php?modulo=inicio&acao=C';</script>");

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$docid = $db->pegaUm("SELECT docid FROM projovemurbano.projovemurbano WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");

if(!$docid) {
	$docid = criaDocumento();
	$db->executar("UPDATE projovemurbano.projovemurbano SET docid='".$docid."' WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$db->commit();
}

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuProJovemUrbano(), "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A");


monta_titulo('Projovem Urbano', montaTituloEstMun());

switch( $_GET['aba'] )
{
	case 'coordenadorResponsavel':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel";
		$pagAtiva = "coordenadorResponsavel.inc";			
		break;
	case 'metaMatriculaInicioAula':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula";
		$pagAtiva = "metaMatriculaInicioAula.inc";			
		break;
	case 'poloNucleo':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo";
		$pagAtiva = "poloNucleo.inc";			
		break;
	case 'profissionais':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais";
		$pagAtiva = "profissionais.inc";			
		break;
	case 'formacaoEducadores':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores";
		$pagAtiva = "formacaoEducadores.inc";			
		break;
	case 'generoAlimenticios':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios";
		$pagAtiva = "generoAlimenticios.inc";			
		break;
	case 'qualificacaoProfissionalEstado':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissionalEstado";
		$pagAtiva = "qualificacaoProfissionalEstado.inc";			
		break;
	case 'qualificacaoProfissionalMunicipio':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissionalMunicipio";
		$pagAtiva = "qualificacaoProfissionalMunicipio.inc";			
		break;
	case 'transporteDidatico':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico";
		$pagAtiva = "transporteDidatico.inc";			
		break;
	case 'demaisAcoes':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes";
		$pagAtiva = "demaisAcoes.inc";			
		break;
	case 'resumoFinanceiro':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro";
		$pagAtiva = "resumoFinanceiro.inc";
		break;
	case 'repasseRecurso':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=repasseRecurso";
		$pagAtiva = "repasseRecurso.inc";
		break;
	case 'visualizarPlano':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=visualizarPlano";
		$pagAtiva = "visualizarPlano.inc";
		break;
	default:
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel";
		$pagAtiva = "coordenadorResponsavel.inc";			
		break;
}

echo "<br>";

echo montarAbasArray(montaMenuPlanoImplementacao(), $abaAtiva);

include $pagAtiva;

?>