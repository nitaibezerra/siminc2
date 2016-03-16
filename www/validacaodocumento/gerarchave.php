<?php
require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . 'elabrev/classes/modelo/HtmlToPdf.class.inc';

/*
 * Função para gerar uma chave única de validação de 
 * documentos PDF gerados no SIMEC.
 * A chave será única na tabela public.validacaodocumento
 * Parâmentros: sisid, html
 * Retorno: vldid
 */
function gerarDocumentoValidado($html) {
    global $db;

    /* Gerando a Chave */
    $chave = null;
    
    do {
        $chave = $db->pegaUm("SELECT
                SUBSTR( UPPER( md5( random()::text ) ) , 0 , 20 )
                WHERE
                SUBSTR( UPPER( md5( random()::text ) ) , 0 , 20 ) NOT IN
                (
                SELECT
                   vldchave
                FROM
                   public.validacaodocumento )
                ");
    } while (empty($chave));

	$simec = "	<div style='border: 1px solid black; float: right; font-size: 12px; padding: 5px; width: 350px;'>";
	$simec.= "		<div style='float: left; width: 327px;'>Documento validado SIMEC código: <b>{$chave}</b>";
	$simec.= "			<br/>Acesse <b>www.simec.mec.gov.br</b> para verificar a autenticidade.";
	$simec.= "		</div>";
	$simec.= "		<div style='float: left; width: 20px; font-size: 26px; color: green; font-weight: bold;'>&#10003;</div>";
	$simec.= "	</div>";
	
	$output = str_replace('{{simec_chave}}', $simec, $html);
	
    $documento = new HtmlToPdf($output);
    $documento->setTitle("{$chave}.pdf");
    $pdf = $documento->getContent();
    $temp = tempnam(sys_get_temp_dir(), 'Pdf');
    
    $fp = fopen($temp, "w+");
    stream_set_write_buffer($fp, 0);
    fwrite($fp, $pdf);
    
    $_FILES['pdf'] = array();
    $_FILES['pdf']['name'] = "{$chave}.pdf";
    $_FILES['pdf']['type'] = 'application/pdf';
    $_FILES['pdf']['tmp_name'] = $temp;
    $_FILES['pdf']['size'] = filesize($temp);
    
    fclose($fp);
        
	$campos = array
	(
		"sisid" => "'" . $_SESSION['sisid'] . "'",
        "vldchave" => "'" . $chave . "'",
        "dataultimaatualizacao" => "'" . date('Y-m-d H:i:s') . "'",
        "usucpf" => "'" . $_SESSION['usucpf'] . "'",
	);
    
    $arquivo = new FilesSimec('validacaodocumento', $campos, 'public');
    $arquivo->setCopiar(true);
    $arquivo->setUpload($chave, 'pdf', true, 'vldid');
    
    return $arquivo->getCampoRetorno();
}

/*
 * Baixa um documento que já foi gerado
 */
function baixarDocumentoValidado($vldid) {
	global $db;

	$stmt = sprintf("select vldid, arqid from public.validacaodocumento where vldid = '%s'", $vldid);
	$row = $db->pegaLinha($stmt);

	$arquivo = new FilesSimec('validacaodocumento', $campos, 'public');
	$arquivo->getDownloadArquivo($row['arqid']);
}

/*
 * Baixa uma imagem verificando no cache
 */
function baixarImagem($key, $pathImg){
	$fileContent = file_get_contents($pathImg);
	$res = base64_encode($fileContent);
	
	return $res;
	
	$tempocache = 86400;
	
	try {
		global $memcache_obj;
		if (!$memcache_obj) $memcache_obj = memcache_connect($GLOBALS["memcachehost"], $GLOBALS["memcacheport"]);
		$cache_result = memcache_get($memcache_obj, $key);

		if ($cache_result) {
			$res = $cache_result;
		} else {
			if(file_exists( $pathImg )){
				$fileContent = file_get_contents($pathImg);
				$res = base64_encode($fileContent);
				memcache_set($memcache_obj, $key, $res, 0, $tempocache);
			}
		}
	} catch (Exception $e){
		if(file_exists( $pathImg )){
			$fileContent = file_get_contents($pathImg);
		}
	}
	
	return $res;
}

?>