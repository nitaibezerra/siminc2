<?php
switch ($_REQUEST['requisicao_upload']) {
    case 'upload_arquivo':
        $file = new FilesSimec();
        if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
            $dados = array(
                'arqid' => $file->getIdArquivo(),
                'arqnome' => $_FILES['file']['name'],
                'arqdescricao' => $_REQUEST['arqdescricao'],
                'arqmdid' => $_REQUEST['arqmdid']
            );
            echo simec_json_encode($dados);
            $cArquivoModulo = new Public_Controller_ArquivoModulo();
            $cArquivoModulo->salvar($dados);
        }else{
            echo simec_json_encode(array(
                'error' => 1,
                'errorMensage' => 'Não foi possível enviar o arquivo!'
            ));
        }
        die;
    case 'listar_arquivos_modulo':
        $arquivoModulo = new Public_Model_ArquivoModulo();
        $listaArquivos = $arquivoModulo->recuperaArquivosPorModulo();
        include_once APPRAIZ. "public/lista_arquivos_modulo.inc";
        die;
    case 'remover_arquivos_modulo':
        $cArquivoModulo = new Public_Controller_ArquivoModulo();
        $listaArquivos = $cArquivoModulo->excluir($_REQUEST);
        die;   
    case 'download_arquivos_modulo':
        $file = new FilesSimec();
        $file->getDownloadArquivo($_REQUEST['arqid']);
	die('<script type="text/javascript">
                document.location.href = document.location.href;
             </script>');
}
