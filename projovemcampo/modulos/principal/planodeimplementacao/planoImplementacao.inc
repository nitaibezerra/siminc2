<?php 
// if($_SESSION['projovemcampo']['ppuid'] != '3'){
// 	echo "<script>
// 			alert('Falha na verificação do ano de exercicio.');
// 			window.location.href = 'projovemcampo.php?modulo=inicio&acao=C';
// 		  </script>";
// 	die();
// }

if(!$_SESSION['projovemcampo']['apcid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemcampo.php?modulo=inicio&acao=C';</script>");

include_once APPRAIZ."/www/projovemcampo/_funcoes_planoimplementacao.php";

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

echo montarAbasArray(montaMenuprojovemcampo(), "projovemcampo.php?modulo=principal/planoimplementacao&acao=A");

monta_titulo('Projovem Urbano', montaTituloEstMun());

switch( $_GET['aba'] )
{
	case 'tramitaPlanos';
		$abaAtiva = "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba=tramitaPlanos";
		$pagAtiva = "tramitaPlanos";
		unset($_SESSION['projovemcampo']['tprid']);
		break;
	default:
		$abaAtiva = "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba=metaMatriculaInicioAula";
		$pagAtiva = "metaMatriculaInicioAula";
		break;
}

echo "<br>";




		$menu[] = array("id" => 1,
                   		"descricao" => 'Meta, Matrícula e Início de aula',
                    	"link" => "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba=".$programa['aba']);
        $menu[] = array("id" => 2,
		        		"descricao" => 'Tramitação - Plano de Implementação',
		        		"link" => "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba=tramitaPlanos");
        
echo montarAbasArray($menu, $abaAtiva);
monta_titulo(' ', '');
if( $_GET['aba'] != '' && $_GET['aba']!= 'tramitaPlanos'&& $_GET['aba']!= 'enderecoEntrega'){
	switch( $_REQUEST['aba2'] )
	{
		case 'metaMatriculaInicioAula':
			$abaAtiva2 = "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba=metaMatriculaInicioAula";
			$pagAtiva = "metaMatriculaInicioAula";			
			break;
		default:
			$abaAtiva2 = "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba={$_GET['aba']}&aba2=metaMatriculaInicioAula";
			$pagAtiva = "metaMatriculaInicioAula";			
			break;
	}
	
	echo "<br>";
	
	$menu2[] = array("id" => 1, 
					"descricao" => "Meta, Matrícula e Início de aula", 
					"link" => "projovemcampo.php?modulo=principal/planoimplementacao&acao=A&aba={$_GET['aba']}&aba2=metaMatriculaInicioAula");
	monta_titulo(' ', ''); 
	
}
include 'planoImplementacao/'.$pagAtiva.inc';


?>

