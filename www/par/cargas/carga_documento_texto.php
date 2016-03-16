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

if( $_REQUEST['mostraarquivo'] == 'S' ){
	$path = APPRAIZ."arquivos/par/documentoTermo/"; 
	$diretorio = dir($path); 
	$tot = 1;
	echo "Lista de Arquivos do diretório '<strong>".$path."</strong>':<br />";
	
	while($arquivo = $diretorio -> read()){ 
		if( $arquivo != '.' && $arquivo != '..' ){
			echo "<a href='".$path.$arquivo."'>".$tot.' - '.$arquivo."</a><br />";
			$tot++;
		}
	} 
	$diretorio -> close();
	
} else {

	$sql = "SELECT dp.dopid, dp.prpid, dp.proid, dp.doptexto, dp.dopstatus
			FROM par.documentopar dp 
			WHERE dopid not in (select dopid from par.documentotermoarquivo t where dopid is not null) limit 5000";
	
	$arMinuta = $db->carregar( $sql );
	$arMinuta = $arMinuta ? $arMinuta : array();
	
	if( !empty($arMinuta[0]) ){
		foreach ($arMinuta as $v){
			
			if( !empty($v['prpid']) ){
				$tipo = 'PAR';
				$codigo = $v['prpid']; 
			} else {
				$tipo = 'OBRA';
				$codigo = $v['proid'];
			}
			
			gravaHtmlDocumento( $v['doptexto'], $v['dopid'], $codigo, $tipo);
			
			$db->executar("update par.documentotermoarquivo set dtastatus = '{$v['dopstatus']}' where dopid = {$v['dopid']}");
			$db->commit();
		}
		$total = (float)($_REQUEST['registro'] ? $_REQUEST['registro'] : 0) + (float)sizeof($arMinuta);
		/*echo "<script>
					window.location.href = 'cargas/carga_documento_texto.php?registro=".$total."';
				</script>";*/
		exit();
	}
}