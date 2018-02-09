<?php
if($_REQUEST['gerar']){

    set_time_limit(0);

    define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

    $obras = array();

    $_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
    // $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

    // carrega as funções gerais
    require_once BASE_PATH_SIMEC . "/global/config.inc";
    // require_once "../../global/config.inc";

    require_once APPRAIZ . "includes/classes_simec.inc";
    require_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
    include_once APPRAIZ . "includes/classes/Sms.class.inc";

    //eduardo - envio SMS pendecias de obras - PAR
    //http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
    // CPF do administrador de sistemas
    $_SESSION['usucpforigem'] = '00000000191';
    $_SESSION['usucpf'] = '00000000191';
    $_SESSION['sisid'] = 98;

    $nomeEsquema = 'par';
    $db = new cls_banco();

    $sql = "select  bn.bncid, bna.bnaid, bna.arqid, arq.arqnome || '.' || arq.arqextensao as nome_arquivo, iu.itrid, case when iu.itrid = 1 then 'Estadual' else 'Municipal' end esfera,
                case when iu.itrid = 1 then iu.estuf else mun.estuf || ' - ' || mun.mundescricao end descricao,
                case when iu.itrid = 1 then iu.estuf else mun.estuf  end uf
            from par.basenacionalcomum bn
                    inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                    left join territorios.municipio mun on mun.muncod = iu.muncod
                    inner  join par.basenacionalcomumarquivo bna on bna.bncid = bn.bncid
                    inner  join public.arquivo arq on arq.arqid = bna.arqid
            order by esfera, uf, descricao";

    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();

    $dadosAgrupados = array();
    foreach($dados as $dado){
        $dadosAgrupados[$dado['esfera']][$dado['uf']][$dado['descricao']][] = $dado;
    }

    $pathRaiz = APPRAIZ."arquivos/".$nomeEsquema.'/base_nacional/';
    $origem = APPRAIZ."arquivos/".$nomeEsquema;

    if(!is_dir($pathRaiz)){
        mkdir($pathRaiz, 0777);
    }

    foreach($dadosAgrupados as $esfera => $ufs){

        if(!is_dir($pathRaiz . '/' . $esfera)){
            mkdir($pathRaiz . '/' . $esfera, 0777);
        }

        foreach($ufs as $uf => $descricoes){
            if(!is_dir($pathRaiz . '/' . $esfera . '/' . $uf)){
                mkdir($pathRaiz . '/' . $esfera . '/' . $uf, 0777);
            }
            foreach($descricoes as $descricao => $arquivos){
                $descricao = removeAcentos($descricao);

                $pastaFinal = $pathRaiz . '/' . $esfera . '/' . $uf . '/' . $descricao;
                if(!is_dir($pastaFinal)){
                    mkdir($pastaFinal, 0777);
                }

                foreach($arquivos as $arquivo){

                    $arqid = $arquivo['arqid'];
                    $caminhoOrigem  = $origem . '/' . floor($arqid/1000) . '/' . $arqid;
                    $caminhodestino = removeAcentos($pastaFinal . '/' . $arquivo['nome_arquivo']);
                    if(is_file($caminhoOrigem)){
                        $resultado = copy($caminhoOrigem, $caminhodestino);
                    }
                }
            }
        }
    }

    /*

    // Get real path for our folder
    $rootPath = realpath($pathRaiz . '/Estadual');

    // Initialize archive object
    $zip = new ZipArchive;
    $zip->open('file.zip', ZipArchive::CREATE);

    // Initialize empty "delete list"
    $filesToDelete = array();

    // Create recursive directory iterator
    $files = new RecursiveIteratorIterator(
        new RecursiveDirectoryIterator($rootPath),
        RecursiveIteratorIterator::LEAVES_ONLY
    );
    ver($files, d);
    foreach ($files as $name => $file) {
        // Get real path for current file
        $filePath = $file->getRealPath();

        // Add current file to archive
        $zip->addFile($filePath);

        // Add current file to "delete list" (if need)
        if ($file->getFilename() != 'important.txt')
        {
            $filesToDelete[] = $filePath;
        }
    }

    // Zip archive will be created only after closing object
    $zip->close();

    // Delete all files from "delete list"
    foreach ($filesToDelete as $file)
    {
        unlink($file);
    }


    $resultado = copy($origem, $destino);

    ver($resultado, $origem, $destino, d);
    */

    echo "FIM";
//    ver($dadosAgrupados, d);
}