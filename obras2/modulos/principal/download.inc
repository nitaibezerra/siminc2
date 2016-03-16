<?php

function getArquivo($arqId) {
    global $db;
    $strSQL = "SELECT * FROM public.arquivo WHERE arqid = {$arqId}";
    $linha = $db->pegaLinha($strSQL);
    return ($linha) ? $linha : false;
}

function pegaArquivoDownload($arqId) {
    
    if ($dadosArquivo = getArquivo($arqId)) {
        
        require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
        $file = new FilesSimec(null, null, 'obras2');
        $filepath = $file->getCaminhoFisicoArquivo($arqId);
        if (file_exists($filepath)) {

            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/excel; name={$dadosArquivo['arqnome']}.{$dadosArquivo['arqextensao']}");
            header("Content-Disposition: attachment; filename={$dadosArquivo['arqnome']}.{$dadosArquivo['arqextensao']}");
            //header("Content-Description: MID Gera excel");
            echo file_get_contents($filepath);
        }
    }
}

if ($_GET['arqid'] && $_GET['secao']) {
    switch ($_GET['secao']) {
        case 'relatorio':
            pegaArquivoDownload($_GET['arqid']);
            break;
    }
}