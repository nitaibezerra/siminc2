<?php
$perfis = pegaPerfilGeral();
if ($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){

	if ($_REQUEST['estuf']){
		$_SESSION['projovemcampo']['estuf'] = $_REQUEST['estuf'];
	} else if ($_REQUEST['muncod']){
		$_SESSION['projovemcampo']['muncod'] = $_REQUEST['muncod'];
	}
		carregarProjovemCampo();
}

if(!$_SESSION['projovemcampo']['apcid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemcampo.php?modulo=inicio&acao=C';</script>");

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
$perfis = pegaPerfilGeral();

$docid = $db->pegaUm("SELECT docid FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");

if(!$docid) {
	$docid = criaDocumento();
	$db->executar("UPDATE projovemcampo.adesaoprojovemcampo SET docid='".$docid."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
	$db->commit();
}
$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

monta_titulo('Projovem Campo', montaTituloEstMun());
// ver(montaMenuprojovemcampo(),d);
echo montarAbasArray(montaMenuprojovemcampo(), "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A");

// if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){
// 	$_REQUEST['aba'] = 'turma';
// }
$us = pegarUsuarioProJovemCampo();

$rsMetas = recuperaMetasPorUfMuncod($us);

$sql = "SELECT 	
			s.secoordcpf
		FROM 
					projovemcampo.secretaria s
		INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = s.secaid
		WHERE 
			apcid='".$_SESSION['projovemcampo']['apcid']."'";

$coordenadorresponsavel = $db->pegaUm($sql);

switch( $_REQUEST['aba'] )
	{
		case 'coordenadorResponsavel':
// 			if(!in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)&&!in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){
				$abaAtiva = "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=coordenadorResponsavel";
				$pagAtiva = "coordenadorResponsavel";
// 			}
			break;
			case 'turma':
		if($coordenadorresponsavel){
				$abaAtiva = "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=turma";
				$pagAtiva = "turma";
		}
				break;
		default:
			if(!in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)&&!in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){
				$abaAtiva = "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=coordenadorResponsavel";
				$pagAtiva = "coordenadorResponsavel";
			}else{
				$abaAtiva = "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=turma";
				$pagAtiva = "turma";
			}
			break;
	}

		echo "<br>";
// 	if(!in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)&&!in_array(PFL_COORDENADOR_ESTADUAL, $perfis)){
		$menu[] = array("id" => 1,
				"descricao" => "Coordenador Responsável",
				"link" => "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=coordenadorResponsavel");
// 	}
	if ($coordenadorresponsavel) {
		$menu[] = array("id" => 2,
				"descricao" => "Escola/Turma",
				"link" => "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=turma");
	
	}
echo montarAbasArray($menu, $abaAtiva);
// monta_titulo(' ', '');
// ver($pagAtiva,d);
include $pagAtiva.'.inc';
?>
