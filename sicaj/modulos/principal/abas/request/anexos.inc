<?php

if ($_GET['deleteFile']) {

    $arqid = $db->pegaUm("select arqid from sicaj.anexogeral where arqid = {$_GET['deleteFile']}");
    if ($arqid) {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec("anexogeral", array(''), "sicaj");
        $file->setRemoveUpload($arqid);
        $_SESSION['flashmessagem'] = array(
            'message' => 'Arquivo apagado com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );
    } else {
        $_SESSION['flashmessagem'] = array(
            'message' => 'Falha ao tentar apagar o arquivo!',
            'type' => Simec_Helper_FlashMessage::ERRO
        );
    }

    die("<script type='text/javascript'>
        location.href='{$_SERVER['HTTP_REFERER']}';
    </script>");
}

$acceptedExtesion = array('pdf');

if (count($_FILES) && is_numeric($_GET['id'])) {

    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

    if ($_FILES['cpDecisaoJudicial']['size'] && $_FILES['cpDecisaoJudicial']['error']=='0') {

        $fileinfo = pathinfo($_FILES['cpDecisaoJudicial']['name']);

        if (!in_array(strtolower($fileinfo['extension']), $acceptedExtesion)) {
            $cpDecisaoJudicial = null;
        } else {
            $campos = array(
                'pdcid' => $_GET['id'],
                'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
                'arpstatus' => "'A'",
                'arpdsc' => "''",
                'angtipoanexo' => "'DJ'",
                'arptipo' => "''",
                'angdsc' => "''"
            );

            $file = new FilesSimec('anexogeral', $campos ,'sicaj');
            $cpDecisaoJudicial = $file->setUpload(null, 'cpDecisaoJudicial');
        }
    }

    if ($_FILES['cpParecerExecutorio']['size'] && $_FILES['cpParecerExecutorio']['error']=='0') {

        $fileinfo = pathinfo($_FILES['cpParecerExecutorio']['name']);

        if (!in_array(strtolower($fileinfo['extension']), $acceptedExtesion)) {
            $cpParecerExecutorio = null;
        } else {
            $campos = array(
                'pdcid' => $_GET['id'],
                'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
                'arpstatus' => "'A'",
                'arpdsc' => "''",
                'angtipoanexo' => "'PE'",
                'arptipo' => "''",
                'angdsc' => "''"
            );

            $file = new FilesSimec('anexogeral', $campos ,'sicaj');
            $cpParecerExecutorio = $file->setUpload(null, 'cpParecerExecutorio');
        }
    }

    if ($_FILES['cpPlanilhaFinanceira']['size'] && $_FILES['cpPlanilhaFinanceira']['error']=='0') {

        $fileinfo = pathinfo($_FILES['cpPlanilhaFinanceira']['name']);

        if (!in_array(strtolower($fileinfo['extension']), $acceptedExtesion)) {
            $cpPlanilhaFinanceira = null;
        } else {
            $campos = array(
                'pdcid' => $_GET['id'],
                'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
                'arpstatus' => "'A'",
                'arpdsc' => "''",
                'angtipoanexo' => "'PF'",
                'arptipo' => "''",
                'angdsc' => "''"
            );

            $file = new FilesSimec('anexogeral', $campos ,'sicaj');
            $cpPlanilhaFinanceira = $file->setUpload(null, 'cpPlanilhaFinanceira');
        }
    }

    if ($_FILES['outrosDocumentos']['size'] && $_FILES['outrosDocumentos']['error']=='0') {

        $fileinfo = pathinfo($_FILES['outrosDocumentos']['name']);

        $campos = array(
            'pdcid' => $_GET['id'],
            'arpdtinclusao' => "'".date('Y-m-d H:i:s')."'",
            'arpstatus' => "'A'",
            'arpdsc' => "''",
            'angtipoanexo' => "'OD'",
            'arptipo' => "''",
            'angdsc' => "''"
        );

        $file = new FilesSimec('anexogeral', $campos ,'sicaj');
        $outrosDocumentos = $file->setUpload(null, 'outrosDocumentos');
    }

    if ($cpDecisaoJudicial || $cpParecerExecutorio || $cpPlanilhaFinanceira || $outrosDocumentos) {
        $fm->addMensagem('Upload concluído com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha no upload do arquivo!', Simec_Helper_FlashMessage::ERRO);
    }

    $_FILES = $_POST = array();
    die("<script type='text/javascript'>
        location.href='{$_SERVER['HTTP_REFERER']}';
    </script>");
}
