<?php

include_once APPRAIZ . "www/obras/_funcoes.php";

$res = obras_pegarOrgaoPermitido();

if ( empty( $_GET['org'] ) ){
	$_REQUEST['org'] = $_SESSION['pesquisaObra']["org"];
}

$org = array();

if ($_REQUEST['org']){
	$org = (array) $_REQUEST['org'];
}elseif (is_array($res)){
	foreach ($res as $r){
		if ($r['id']) $org[] = $r['id'];
	}
}

if (  $_SESSION['obra']['orgid'] == ORGAO_FNDE ){
	
	$podeVer = verificaPermissaoObra( $_SESSION["usucpf"], $_REQUEST["obrid"], $_SESSION['obra']['orgid'] );
	if ( !$podeVer ){
		echo "<script>
				alert('Você não possui permissão para ver esta obra!');
				history.back(-1);
			  </script>";
		die;
	}
	
}

$arPerfilEntid = array( PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE, PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR );
$arPerfilOrgid = array( /*PERFIL_EMPRESA,*/ PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR );

if( $org )
{
	$habilitado = obras_possuiPerfilOrgao( $arPerfilEntid, $arPerfilOrgid, $org );
}

$arrMenuNotBloq = array(
						'inicio',
						'principal/etapas_da_obra'
						);

if ( !in_array($_GET['modulo'] , $arrMenuNotBloq) ){						
	//if ($habilitado) $habilitado = obraAditivoPossuiCronograma();
	if ($habilitado) $habilitado = obraAditivoPossuiVistoria();
}
$somenteLeitura = $habilitado  ? 'S' : 'N';
$disabled = $habilitado ? '' : 'disabled';

/*** Se for uma cópia de uma obra desabilita os campos ***/
if( verificaCopiaObra($_REQUEST["obrid"]) )
{
	$habilitado = false;
}

$boReformulacao = false;

if( $_SESSION['obra']['obrid'] || $_REQUEST["obrid"] ){
	
	$obrid = ($_REQUEST["obrid"]) ? $_REQUEST["obrid"] : $_SESSION['obra']['obrid'];
	if( !is_array($obrid) ){
		$stoid = $db->pegaUm( "SELECT stoid FROM obras.obrainfraestrutura WHERE obrid = ".$obrid." and obsstatus = 'A'" );
		if( $stoid == '9' ){
			$boReformulacao = true;
			$somenteLeitura = 'N';
			$habilitado = false;
		}
		
		#Verifica se no parecer do ckeck list esta aba foi bloqueiada
		$boBloqueiaAbaCleckList = verificaCheckListBloqueiaAba($titulo_modulo, $obrid);
		$boBloqueioAba = false;
		
		//if( $boBloqueiaAbaCleckList[0] == 't' && possuiPerfil(array(PERFIL_EMPRESA)) ){
		if( in_array(PERFIL_EMPRESA, obras_arrayPerfil() ) ){
			if( $boBloqueiaAbaCleckList[0] == 't' ){
				$somenteLeitura = 'N';
				$habilitado = false;
				$boBloqueioAba = true;
			}
			
			$esdid = $db->pegaUm( "SELECT esdid 
								   FROM obras.obrainfraestrutura o 
								   INNER JOIN workflow.documento d ON d.docid = o.docid
								   WHERE obrid = ".$obrid." 
								   and obsstatus = 'A'" );
			//240 = "Em Supervisão (Empresa)"
			//242 = "Ajuste de Supervisão (Empresa)"
			//279 = "Reajuste de Supervisão (Empresa)"
			if( $esdid != 240 && $esdid != 242 && $esdid != 279){
				$somenteLeitura = 'N';
				$habilitado = false;
				$boBloqueioAba = true;
			}
		
		}
	
		//VERIFICA SE A OBRA FOI MIGRADA E BLOQUEIA CASO AFIRMATIVO
		$obraMigrada = $db->pegaUm("SELECT obrid FROM obras2.obras WHERE obrid_1 = ".$obrid." AND obrstatus = 'A'" );
		if($obraMigrada){
			$somenteLeitura = 'S';
			$habilitado = false;
			$boBloqueioAba = true;
		}
	}
}

$disabled = $habilitado ? '' : 'disabled';
?>