<?php

if ($_GET['deleteFile']) {

    $arqid = $db->pegaUm("select arqid from sicaj.anexogeral where arqid = {$_GET['deleteFile']}");
    if ($arqid) {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec("anexogeral", array(''), "sicaj");
        $file->setRemoveUpload($arqid);
        $fm->addMensagem('Arquivo apagado com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha ao tentar apagar o arquivo!', Simec_Helper_FlashMessage::ERRO);
    }
}
$acceptedExtension = array('pdf');
if (!empty($_FILES) && is_numeric($_GET['id']) && (PERFIL_UO_EQUIPE_TECNICA != pegarPerfilAtual($_SESSION['usucpf']))) {

    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    if (count($_FILES)) {
        if ($_FILES['homologacaoSICAJ']['size'] && $_FILES['homologacaoSICAJ']['error']=='0') {

            $fileinfo = pathinfo($_FILES['homologacaoSICAJ']['name']);
            if (!in_array(strtolower($fileinfo['extension']), $acceptedExtension)) {
                $homologacaoSICAJ = null;
            } else {
                $campos = array(
                    'pdcid' => $_GET['id'],
                    'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
                    'arpstatus' => "'A'",
                    'arpdsc' => "''",
                    'angtipoanexo' => "'HS'",
                    'arptipo' => "''",
                    'angdsc' => "''"
                );

                $file = new FilesSimec('anexogeral', $campos ,'sicaj');
                $homologacaoSICAJ = $file->setUpload(null, 'homologacaoSICAJ');
            }
        }

        if ($_FILES['outrosDocumentos']['size'] && $_FILES['outrosDocumentos']['error']=='0') {
            $fileinfo = pathinfo($_FILES['outrosDocumentos']['name']);
            if (!in_array(strtolower($fileinfo['extension']), $acceptedExtension)) {
                $outrosDocumentos = null;
            } else {
                $campos = array(
                    'pdcid' => $_GET['id'],
                    'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
                    'arpstatus' => "'A'",
                    'arpdsc' => "''",
                    'angtipoanexo' => "'O2'",
                    'arptipo' => "''",
                    'angdsc' => "''"
                );

                $file = new FilesSimec('anexogeral', $campos ,'sicaj');
                $outrosDocumentos = $file->setUpload(null, 'outrosDocumentos');
            }
        }

        if ($_FILES['msgEletronicaSEGEP']['size'] && $_FILES['msgEletronicaSEGEP']['error']=='0') {
            $fileinfo = pathinfo($_FILES['msgEletronicaSEGEP']['name']);
            if (!in_array(strtolower($fileinfo['extension']), $acceptedExtension)) {
                $msgEletronicaSEGEP = null;
            } else {
                $campos = array(
                    'pdcid' => $_GET['id'],
                    'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
                    'arpstatus' => "'A'",
                    'arpdsc' => "''",
                    'angtipoanexo' => "'MS'",
                    'arptipo' => "''",
                    'angdsc' => "''"
                );

                $file = new FilesSimec('anexogeral', $campos ,'sicaj');
                $msgEletronicaSEGEP = $file->setUpload(null, 'msgEletronicaSEGEP');
            }
        }

        if ($homologacaoSICAJ || $msgEletronicaSEGEP || $outrosDocumentos) {
            $fm->addMensagem('Upload concluído com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
        } else {
            $fm->addMensagem('Falha no upload do arquivo!', Simec_Helper_FlashMessage::ERRO);
        }
    }

    $_FILES = $_POST = array();
    die("<script type='text/javascript'>
        location.href='{$_SERVER['HTTP_REFERER']}';
    </script>");
}
