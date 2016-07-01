<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_REQUEST['arq_iclid'] && $_REQUEST['arq_tpvid'] && $_REQUEST['arq_entid'] )
{
	$sql = "SELECT 
				vldid 
			FROM 
				pde.validacao 
			WHERE 
				iclid = ".$_REQUEST['arq_iclid']." 
				AND tpvid = ".$_REQUEST['arq_tpvid']."
				AND entid = ".$_REQUEST['arq_entid'];
	$vldid = $db->pegaUm($sql);
	
	if( $vldid )
	{
		$arrCampos = array("vldid" => $vldid);
		$file = new FilesSimec("anexochecklist", $arrCampos, "pde");
			
		$sql = "SELECT arqid FROM pde.anexochecklist WHERE vldid = ".$vldid." AND ancstatus = 'A'";
		$arqid = $db->pegaUm($sql);
		
		if( $arqid )
		{
			$sql = "UPDATE pde.anexochecklist SET ancstatus = 'I' WHERE arqid = ".$arqid;
			$db->executar($sql);
			
			$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid;
			$db->executar($sql);
			
			$db->commit();
			
			//$file->excluiArquivoFisico($arqid);
		}
		
		if( $_FILES['arquivo'] )
		{
			$arqdescricao = 'arquivo_checklist_pacto_'.$vldid;
			
			if( $file->setUpload($arqdescricao, "arquivo") )
			{
				// ok...
			}
		}
	}
}

?>