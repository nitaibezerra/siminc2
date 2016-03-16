<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/par/_funcoesPar.php";
include_once APPRAIZ . "www/par/_funcoes.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '';
$_SESSION['usucpf'] 		= '';

$db = new cls_banco();

$sql = "SELECT dp.terid, dp.proid, dp.terdocumento, dp.terstatus
		FROM par.termocompromissopac dp 
		WHERE terid not in (select terid from par.documentotermoarquivo t where terid is not null) limit 5000";

$arMinuta = $db->carregar( $sql );
$arMinuta = $arMinuta ? $arMinuta : array();

if( !empty($arMinuta[0]) ){
	foreach ($arMinuta as $v){
		
		gravaHtmlDocumento( $v['terdocumento'], $v['terid'], $v['proid'], 'PAC');
		
		$db->executar("update par.documentotermoarquivo set dtastatus = '{$v['terstatus']}' where terid = {$v['terid']}");
		$db->commit();
	}
	$total = (float)($_REQUEST['registro'] ? $_REQUEST['registro'] : 0) + (float)sizeof($arMinuta);
	/*echo "<script>
				window.location.href = 'cargas/carga_documento_texto_pac.php?registro=".$total."';
			</script>";*/
	exit();
}