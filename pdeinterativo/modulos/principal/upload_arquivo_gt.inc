<?php

ini_set('post_max_size', '300M');
ini_set('upload_max_filesize', '300M');
ini_set("memory_limit", "2048M");
set_time_limit(0);

if( $_REQUEST['arq_grtid'] && $_FILES['arquivo'] )
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos = array('tpdid' => TPDID_GRUPO_TRABALHO);
	$file = new FilesSimec("anexo", $campos, "pdeinterativo");
	
	$arqdescricao = 'arquivo_pdeinterativo_gruptrabanexo';
	
	if( $file->setUpload($arqdescricao, "arquivo", true, 'anxid') )
	{
		salvarAbaResposta("primeiros_passos_passo_1");
		
		$sql = "INSERT INTO pdeinterativo.gruptrabanexo(grtid,anxid) VALUES(".$_REQUEST['arq_grtid'].",".$file->getCampoRetorno().")";
		$db->executar($sql);
		$db->commit();
	}
}

?>

<form>
<input type="hidden" id="arq_arqid" name="arq_arqid" value="<?=$file->getIdArquivo()?>" />
</form>