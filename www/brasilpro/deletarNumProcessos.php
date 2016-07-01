<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();


$inuid = $_SESSION['inuid'];
if($inuid){	
	try{
		$sql = "delete from cte.subacaoconvenio where cnvid in ( select cnvid from cte.convenio where inuid = $inuid )";

		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir a relação das subações com o convênio." );
		}
		
		$sql = "delete from cte.convenioretorno where cnvid in ( select cnvid from cte.convenio where inuid = $inuid )";

		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir os números de processos." );
		}
		
		$sql = "delete from cte.convenio where inuid =".$inuid;
		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir o convênio." );
		}
		$db->commit();
		echo "<script>alert( 'Dados apagados com sucesso.');
			window.close();
		; </script>";
		return true;
	} catch ( Exception $erro ) {
		$db->rollback();
		echo "<script>alert( '".$erro."');
			window.close();
		; </script>";
	}
}else{
	echo "<script>alert( 'Não foi possível indentificar o instrumento unidade.');'; </script>";
}

?>