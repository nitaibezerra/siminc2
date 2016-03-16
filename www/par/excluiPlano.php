<?php
ini_set("memory_limit","1000M");
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_funcoesPar.php';
include_once '_componentes.php';
$db = new cls_banco();

$strSql = '';
$inserePlano = NULL;
$inseridoPlano = false;
$itrid = 2;
$arrInuid = array(484);


foreach($arrInuid as $inuid){
	// se existe pontuações excluir o plano de ação da entidade.

	$strSql = deletaAcoesSubacoesEntidade($itrid, $inuid, $strSql);

	if($strSql){
			
		$db->executar($strSql);
	}
			
	$sql = "DELETE FROM workflow.historicodocumento WHERE docid in ( select docid from par.instrumentounidade  where inuid = {$inuid}  and docid is not null)";
	if($db->executar($sql)){
		$sql = "DELETE FROM workflow.documento  WHERE tpdid = 44 and docid in ( select docid from par.instrumentounidade  where inuid = {$inuid} and docid is not null)";
		$db->executar($sql);
			
			$sql = "UPDATE par.instrumentounidade SET docid = NULL WHERE inuid = {$inuid} "; //-- docid is not null and estuf is not null
			$db->executar($sql);
		
	}


	echo "excluindo inuid = ".$inuid." <br> ";
}
if($db->commit()){
	echo "excluidos com sucesso.";
	die();
}else{
	echo "ocorreu um erro.";
	
}

$db->close();
?>