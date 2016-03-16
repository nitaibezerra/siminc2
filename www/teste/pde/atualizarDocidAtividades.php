<?php 

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/workflow.php";

error_reporting( E_ALL );

$db = new cls_banco();

function atualizaDocid($post){
	
	global $db;
	
	$sql = "SELECT 
				icl.iclid,
				icl.icldsc,
				a._atinumero,
				a.atidescricao
			FROM 
				pde.atividade a
			INNER JOIN pde.itemchecklist icl ON icl.atiid = a.atiid and docid is null
			WHERE 
				a._atiprojeto = 114098 AND 
				a.atistatus = 'A' AND 
				a.atitipoenem is not null AND 
				a._atinumero like '2.%' ";
				
	$itens = $db->carregar($sql);
	
	if(is_array($itens)){
		foreach($itens as $item){
			
			$docdsc = "<p>".$item['iclid']." - ".$item["icldsc"]."</p><p>".$item['_atinumero']." - ".$item['atidescricao']."</p>";
			$docid = wf_cadastrarDocumento( 39, str_replace("\'","'",$docdsc) );
						
			$db->executar("UPDATE pde.itemchecklist SET docid='".$docid."' WHERE iclid='".$item['iclid']."'");
			
		}
		
		if($db->commit()){
			echo "Docids atualizados em: ";
			foreach($itens as $item){
				$docdsc = "<p>".$item['iclid']." - ".$item["icldsc"]."</p><p>".$item['_atinumero']." - ".$item['atidescricao']."</p>";
				echo $docdsc;
			}
			echo "<script>window.location=window.location;</script>";
		}
	}else{
		echo "Sem itens para atualizar Dicid.";
	}
}

if($_POST){
	$_POST['req']($_POST);
}

?>
<form method="post" name="formDependencia" id="formDependencia">
	<input type="submit" value="atualizaDocid" name="req">
</form>