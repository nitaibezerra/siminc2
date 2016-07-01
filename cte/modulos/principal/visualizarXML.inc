<?php
$sql = "select prsid from cte.projetosape where inuid = ".$_SESSION['inuid']." order by prsid desc limit 1";
$prsid = $db->pegaUm($sql);

if($prsid){
	$sql = "select arqid from cte.convenioxml where inuid = ".$_SESSION['inuid']." and prsid = ".$prsid.";";
	$arqid = $db->pegaUm($sql);
	if($arqid){
		include_once APPRAIZ."includes/classes/fileSimec.class.inc";
		$file 		= new FilesSimec();
		$file->getDownloadArquivo($arqid);
	}else{
		alert("Não existe XML de envio de convênio para o SAPE.");
	}
}else{
	alert("Não existe XML de envio de convênio para o SAPE.");
}


?>