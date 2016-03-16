<?php
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

function montaAbaPublicacao($abaAtivaGeral){
	$db = new cls_banco();
	
	$cpfResp = $db->carregarColuna("SELECT usucpf FROM seguranca.publicacaoresponsavel WHERE purstatus = 'A'");
	
	if( in_array($_SESSION['usucpf'], $cpfResp) ){
		$sql = "SELECT 
						  
						  count(pa.puaid)
					FROM 
					  	seguranca.publicacaoarquivo pa
	                    inner join seguranca.publicacaoresparquivo pra on pra.puaid = pa.puaid
	                    inner join seguranca.publicacaoresponsavel pr on pr.purid = pra.purid
					WHERE
						pa.puasituacao = 'AP'
	                    and pr.usucpf = '{$_SESSION['usucpf']}'";
		$totalPub = $db->pegaUm($sql);
		$totalPub = ($totalPub > 0 ? '<span style="font-weight: bold">('.$totalPub.')</span>' : '');
		
		$menuGeral = array (
				0 => array (
						"id" => 0,
						"descricao" => "Lista Solicitações",
						"link" => "listaPublicarArquivos.php"
				),
				1 => array (
						"id" => 1,
						"descricao" => "Solicitar Publicação",
						"link" => "solicitarPublicacao.php"
				),	
				2 => array (
						"id" => 2,
						"descricao" => "Publicação$totalPub",
						"link" => "publicarArquivos.php"
				)	
		);
	} else {
		
		$menuGeral = array (
				0 => array (
						"id" => 0,
						"descricao" => "Lista Solicitações",
						"link" => "listaPublicarArquivos.php"
				),
				1 => array (
						"id" => 1,
						"descricao" => "Solicitar Publicação",
						"link" => "solicitarPublicacao.php"
				)	
		);
	}
	
	return montarAbasArray ( $menuGeral, $abaAtivaGeral );
}

function copiarArquivos($source, $dest, $options=array('folderPermission'=>0755,'filePermission'=>0755)){
	$result=false;
   
	if (is_file($source)) {
		if ($dest[strlen($dest)-1]=='/') {
			if (!file_exists($dest)) {
				cmfcDirectory::makeAll($dest,$options['folderPermission'],true);
			}
			$__dest=$dest."/".basename($source);
		} else {
			$__dest=$dest;
		}
		$result=copy($source, $__dest);
		chmod($__dest,$options['filePermission']);
	   
	} elseif(is_dir($source)) {
		if ($dest[strlen($dest)-1]=='/') {
			if ($source[strlen($source)-1]=='/') {
				//Copy only contents
			} else {
				//Change parent itself and its contents
				$dest=$dest.basename($source);
				@mkdir($dest);
				chmod($dest,$options['filePermission']);
			}
		} else {
			if ($source[strlen($source)-1]=='/') {
				//Copy parent directory with new name and all its content
				@mkdir($dest,$options['folderPermission']);
				chmod($dest,$options['filePermission']);
			} else {
				//Copy parent directory with new name and all its content
				@mkdir($dest,$options['folderPermission']);
				chmod($dest,$options['filePermission']);
			}
		}

		$dirHandle=opendir($source);
		while($file=readdir($dirHandle))
		{
			if($file!="." && $file!="..")
			{
				 if(!is_dir($source."/".$file)) {
					$__dest=$dest."/".$file;
				} else {
					$__dest=$dest."/".$file;
				}
				//echo "$source/$file ||| $__dest<br />";
				$result=smartCopy($source."/".$file, $__dest, $options);
			}
		}
		closedir($dirHandle);
	   
	} else {
		$result=false;
	}
	return $result;
}
?>