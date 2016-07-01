<?php

/**
 * Verifica se o perfil é o passado por paramêtro
 * @global object $db
 * @param string $perfil
 * @return boolean
 */
function possui_perfil($perfil){

    global $db;

    if( !is_array($perfil) ) $perfil = Array($perfil);
                
    $sql = "select count(*) from seguranca.perfilusuario
            where
                usucpf = '" . $_SESSION['usucpf'] . "' and
                pflcod in ( " . implode( ",", $perfil ) . " ) ";
        
    return $db->pegaUm( $sql ) > 0;

    return (boolean) $db->pegaUm($sql);
}

/**
 * Monta título da página
 * @global object $db
 * @param string $titulo
 * @param int $abacod_tela
 * @param string $url
 * @param string $parametros
 * @param string $subtitulo
 */
function montaTopoPNBE($titulo, $abacod_tela, $url, $parametros, $subtitulo) {
    
    global $db;
    
    // Título
    monta_titulo($titulo, '');
    echo "<br>";
    // Aba Principal
    $db->cria_aba($abacod_tela, $url, $parametros);
    // Subtítulo
    monta_titulo($subtitulo, '');
}

/**
 * Is request xmlHttpRequest
 * @return bool
 */
function isAjax() {
    return (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && 
           strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest');
}
/**
 * Desabilita os botões caso o ano selecionado não seja o ano corrente.
 */
function exercicioCorrente(){
    global $db;
    $sql = "select prsexerccorrente from pnbe.programacaoexercicio where prsano = '{$_SESSION['exercicio']}'";
    $anoCorrente = $db->pegaUm($sql);
    if($anoCorrente == 'f'){
        $status = 'disabled';
    }
   return $status;
}


/**
 * Faz a triagem das obras com update na tabela
 * @global object $db
 * @param array $post
 * @return boolean
 */
function processaTriagemObra(array $request) {
    
    global $db;
    
    if (isset($request['act']) && isset($request['obrid'])) {
        
        switch ($request['act']) {
            case 'add':
                $strSQL = sprintf('UPDATE pnbe.obra SET sitid=%d WHERE obrid=%d', ADICIONA_TRIAGEM, (int)$request['obrid']);
                break;
            case 'del':
                $strSQL = sprintf('UPDATE pnbe.obra SET sitid=%d WHERE obrid=%d', REMOVE_TRIAGEM, (int)$request['obrid']);
                break;
            default:
                return false;
        }
        
        $db->executar($strSQL);
        return $db->commit();
    }
}

/**
 * Pega o status atual da triagem
 * @global object $db
 * @return array
 */
function getStatusTriagem()
{
    global $db;
    
    $statusStrSQL = "SELECT COUNT(*) FROM pnbe.obra WHERE obrano = '{$_SESSION['exercicio']}' and sitid=".ADICIONA_TRIAGEM;
    $statusTriagem = $db->pegaUm($statusStrSQL);
    $cssButton = ($statusTriagem) ? 'enable' : 'disable';
    $textButton = ($statusTriagem) ? 'Finalizar Triagem' : 'Triagem Finalizada';
    
    return array(
        'class' => $cssButton
      , 'text' => $textButton
    );
}

/**
 * Verifica permissão de perfil
 * @global object $db
 * @param array|string $pflcods
 * @return boolean
 */
function possuiPerfil( $pflcods ) {

    global $db;

    if ( is_array( $pflcods ) ){
        $pflcods = array_map( "intval", $pflcods );
        $pflcods = array_unique( $pflcods );
    } else {
        $pflcods = array( (integer) $pflcods );
    } if ( count( $pflcods ) == 0 ) {
        return false;
    }
    
    $sql = "select count(*)
            from 
                seguranca.perfilusuario
            where
                usucpf = '" . $_SESSION['usucpf'] . "' and
                pflcod in ( " . implode( ",", $pflcods ) . " ) ";
    
    return $db->pegaUm( $sql ) > 0;
}

/**
 * Altera status da triagem
 * @global object $db
 * @param string $triagem
 */
function alteraStatusTriagem($triagem)
{
    if (isset($triagem)) {
    
        global $db;
        $strSQLBase = 'UPDATE pnbe.obra SET sitid=%d WHERE sitid=%d';
        
        switch ($triagem) {
            case 'enable':
                $strSQL = sprintf($strSQLBase, TRIAGEM_SELECIONADA, ADICIONA_TRIAGEM);
                $msg = 'Triagem finalizada com sucesso!';
                break;
            case 'disable':
                $strSQL = sprintf($strSQLBase, ADICIONA_TRIAGEM, TRIAGEM_SELECIONADA);
                $msg = 'Triagem habilitada com sucesso!';
                break;
            default:
        }

        $db->executar($strSQL);
        $db->commit();
        
        //header('location:pnbe.php?modulo=principal/triagem&acao=A');
        alertlocation(array(
            'alert' => $msg,
            'location' => 'pnbe.php?modulo=principal/triagem&acao=A'
        ));
    }
}

/**
 * Parametro para filtrar pre-analise
 * @return numeric
 */
function getFilterParam($key, $default = TRIAGEM_SELECIONADA) {
    return isset($_GET[$key]) ? $_GET[$key] : $default;
}

/**
 * 
 * @global object $db
 * @return Array
 */
function getStatusPreAnalise()
{
    global $db;
    
$statusStrSQL = "SELECT COUNT(*) FROM pnbe.obra WHERE obrano = '{$_SESSION['exercicio']}' and sitid=".OBRAS_SELECIONADAS_PREANALISE;
    $statusPreanalise = $db->pegaUm($statusStrSQL);
    $cssButton = ($statusPreanalise) ? 'disable' : 'enable';
    $textButton = ($statusPreanalise) ? 'Pré-análise Finalizada' : 'Finalizar Pré-análise';
    
    return array(
        'class' => $cssButton
      , 'text' => $textButton
    );
}

/**
 * 
 * @param array $params
 * @return boolean
 */
function atualizaObraPreAnalise(array $params) {
    
    global $db;
    
    switch ($params['act']) {
        case 'add':
            //reinclui obra na pre analise
            $strSQL = 'UPDATE pnbe.obra SET sitid=%d WHERE obrid=%d';
            $sitID = TRIAGEM_SELECIONADA;
            ApagaParecerDaObra((int)$params['obrid']);
            break;
        case 'del':
            //excluí da pre-analise
            $strSQL = 'UPDATE pnbe.obra SET sitid=%d WHERE obrid=%d';
            $sitID = OBRAS_EXCLUÍDAS_PREANALISE;
            break;
        default:
    }
    
    $strSQLexec = sprintf($strSQL, $sitID, (int)$params['obrid']);
    $db->executar( $strSQLexec );
    return $db->commit();
}

/**
 * Apaga o parecer da obra quando ela é reicluida na pré-análise
 * @global object $db
 * @param int $obrId
 * @return boolean
 */
function ApagaParecerDaObra($obrId) {
    
    global $db;
    
    $strStmt = 'SELECT o.obrid, o.arqid,
                    a.arqid, a.arqnome, a.arqextensao, a.usucpf
            FROM pnbe.obra o
                    INNER JOIN public.arquivo a ON (o.arqid = a.arqid)
            WHERE o.obrid=%d AND o.sitid=%d';
    
    $strExecQuery = sprintf($strStmt, (int)$obrId, OBRAS_EXCLUÍDAS_PREANALISE);
    $rs = $db->carregar($strExecQuery);
    
    if ($rs) {
        $rs = $rs[0];
        
        apagaArquivoUpload($rs['obrid']);
        updateParecerObra('NULL', $rs['obrid']);
        return true;
    } else
        return false;
}

/**
 * Altera Status da Pré-Análise
 * @param string $param
 * @return boolean
 */
function alteraStatusPreAnalise($param) {
    
    global $db;
    
    switch ($param) {
        //Finaliza pre-analise
        case 'enable':
            $sitid = OBRAS_SELECIONADAS_PREANALISE;
            $where = TRIAGEM_SELECIONADA;
            $msg = 'finlizada';
            break;
        //Reabri pre-analise
        case 'disable':
            $sitid = TRIAGEM_SELECIONADA;
            $where = OBRAS_SELECIONADAS_PREANALISE;
            $msg = 'habilitada';
            break;
        default:
    }
    
    $strSqlExec = sprintf('UPDATE pnbe.obra SET sitid=%d WHERE sitid=%d', $sitid, $where);
    $db->executar( $strSqlExec );
    $db->commit();
    
    alertlocation(array(
        'alert' => "Pré análise $msg com sucesso!",
        'location' => 'pnbe.php?modulo=principal/preanalise&acao=A'
    ));
}

function alertlocation($dados) {
	
	die("<script>
            ".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
            ".(($dados['location'])?"window.location='".$dados['location']."';":"")."
            ".(($dados['javascript'])?$dados['javascript']:"")."
             </script>");
}

function setAvaliacaoObra(array $param) {
    
    if (isset($param['avaliacao']) && isset($param['obrid'])) {
        
        global $db;
        
        $strSqlUpdate = sprintf('UPDATE pnbe.obra SET avaid=%d WHERE obrid=%d', (int)$param['avaliacao'], (int)$param['obrid']);
        $db->executar($strSqlUpdate);
        $db->commit();
        exit;
    }
}

/**
 * Salva a referencia do arquivo na tabela
 * @global object $db
 * @param array $request
 * @return boolean
 */
function salvaArquivoUpload(array $request) {
    global $db;
    
    $strSqlBase = 'INSERT INTO public.arquivo
            (arqnome,
            arqdescricao,
            arqextensao,
            arqtipo,
            arqtamanho,
            arqdata,
            arqhora,
            arqstatus,
            usucpf,
            sisid)
            VALUES
            (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', %s)';
    
    extract($request);
    $strSqlInsert = sprintf($strSqlBase, $arqnome,$arqdescricao,$arqextensao,
            $arqtipo,$arqtamanho,$arqdata,$arqhora,$arqstatus,$usucpf,$sisid);
    
    $db->executar($strSqlInsert);
    return $db->commit();
}

/**
 * Pega o ultimo ID da tabela de arquivo
 * @global object $db
 * @return int
 */
function getLastArqId() {
    global $db;
    $strSqlLastID = 'SELECT MAX(arqid) FROM public.arquivo';
    return $db->pegaUm($strSqlLastID);
}

/**
 * Atualiza a referencia do anexo(Parecer) na obra
 * @global object $db
 * @param int $arqId
 * @param int $obrId
 * @return boolean
 */
function updateParecerObra($arqId, $obrId) {
    global $db;
    $strSql = "UPDATE pnbe.obra SET arqid={$arqId} WHERE obrid={$obrId}";
    $db->executar($strSql);
    return $db->commit();
}

/**
 * Retorna o parecer de uma obra
 * @global object $db
 * @param int $arqId
 * @return Array
 */
function getArquivoParecer($arqId) {
    global $db;
    $strSql = sprintf('SELECT arqnome, arqdescricao FROM public.arquivo WHERE arqid=%s', (int)$arqId);
    return $db->carregar($strSql);
}

/**
 * 
 * @param array $files
 * @return boolean
 */
//function fileUpload(array $files, $describe, $dirname) {
//    
//    $files = $files['fileadd'];
//    $fileinfo = pathinfo($files['name']);
//    $nameFile = $fileinfo['filename'].'_'.time();
//    $destination = $dirname.DS.'uploadFolder'.DS.$nameFile.'.'.$fileinfo['extension'];
//    
//    if (strtolower($fileinfo['extension']) == UPLOAD_VALID_EXENSION) {
//        
//        if (is_uploaded_file($files['tmp_name'])) {
//            if (move_uploaded_file($files['tmp_name'], $destination)) {
//                return array(
//                    'arqnome' => $nameFile, //$fileinfo['filename']
//                    'arqdescricao' => $describe,
//                    'arqextensao' => $fileinfo['extension'],
//                    'arqtipo' => $files['type'],
//                    'arqtamanho' => $files['size'],
//                    'arqdata' => 'NOW()',
//                    'arqhora' => date('h:i:s', time()),
//                    'arqstatus' => 1,
//                    'usucpf' => $_SESSION['usucpf'],
//                    'sisid' => $_SESSION['sisid']
//                );
//            }
//        }
//    }
//    
//    return false;
//}

function apagaArquivoUpload($fileID) {
    
    global $db;
    
    $partialFind = 'SELECT arqid, arqnome, arqextensao, usucpf FROM public.arquivo WHERE arqid=%d';
    $strSqlFind = sprintf($partialFind, (int)$fileID);
    
    if ($fileInfo = $db->carregar($strSqlFind)) {
        
        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
        $file = new FilesSimec( 'obra', NULL, 'pnbe' );
        $file->excluiArquivoFisico($fileID);
    
        return true;
    }
    
    return false;
}

/**
 * Apaga Registro do arquivo na tabela
 * @global object $db
 * @param int $fileID
 * @return boolean
 */
function apagaRegistroArquivo($fileID) {
    
    global $db;
    
    $partialDelete = 'DELETE FROM public.arquivo WHERE arqid=%d';
    $strSql = sprintf($partialDelete, (int)$fileID);
    $db->executar($strSql);
    return $db->commit();
}

/**
 * Verifica se existe pendencia quanto a obras nao avaliadas
 * @global object $db
 * @return string HTML
 */
function verificaPendencia() {
    
    global $db;
    
    //Existe obras não avaliadas
    if (pendenciaAvaliacao()) {
         monstraObrasPendentesAvaliacao();
         ?>
        <script type="text/javascript"> notice("Obras pendentes de avaliação!"); </script>
         <?php
    } else {
        //Todas as obras já foram avaliadas
        monta_titulo('Todas as obras já foram avaliadas', '');
        echo "<br>";
    }
    exit;
}

/**
 * Verifica se existe obras com pendencia na avaliacao
 * @global object $db
 * @param boolean $count
 * @return bool|int
 */
function pendenciaAvaliacao($count = false) {
    
    global $db;
    
    $strSqlCount = "SELECT COUNT('avaid') FROM pnbe.obra WHERE (avaid=".OBRAS_NAO_AVALIADAS." OR avaid is null) AND sitid=".OBRAS_SELECIONADAS_PREANALISE;
    $return = $db->pegaUm($strSqlCount);
    
    if ($count) {
        return $return;
    } else {
        return ($return) ? true : false;
    }
}

/**
 * Mostra um Grid com as obras pendentes de Avaliação
 * @global object $db
 * @return String (HTML)
 */
function monstraObrasPendentesAvaliacao() {
    
    global $db;
    
    //$count = pendenciaAvaliacao(true);
    $strSqlSelect = 'SELECT 
            c.catdescricao, o.obrcodigo, o.obrtitulo 
            FROM pnbe.obra o
            INNER JOIN pnbe.categoria c ON (c.catid = o.catid)
            WHERE (avaid='.OBRAS_NAO_AVALIADAS.' OR avaid is null) AND sitid='.OBRAS_SELECIONADAS_PREANALISE;
    $cabecalho = array('Categoria', 'Código da obra', 'Título da obra');
    $db->monta_lista($strSqlSelect,$cabecalho,100000000,20,'','','','');
    
}

/**
 * Verifica se existe obras com pendencia no parecer(anexo)
 * @global object $db
 * @return bool|int
 */
function pendenciaParecer($count = false) {
    
    global $db;
    
    $strSqlCount = "SELECT COUNT('arqid') FROM pnbe.obra WHERE arqid IS NULL AND sitid=".OBRAS_SELECIONADAS_PREANALISE;
    $return = $db->pegaUm($strSqlCount);
    
    if ($count) {
        return $return;
    } else {
        return ($return) ? true : false;
    }
}

/**
 * Imprime um grid com as obras pendentes de parecer(anexo)
 * @global object $db
 * @return String
 */
function mostraObrasPendentesParecer() {
    global $db;
    
    $count = pendenciaAvaliacao(true);
    $strSqlSelect = 'SELECT 
            c.catdescricao, o.obrid, o.obrtitulo 
            FROM pnbe.obra o
            INNER JOIN pnbe.categoria c ON (c.catid = o.catid)
            WHERE o.arqid IS NULL AND o.sitid='.OBRAS_SELECIONADAS_PREANALISE;
    
    $cabecalho = array('Categoria', 'Código da obra', 'Título da obra');
    $db->monta_lista($strSqlSelect,$cabecalho,1000000,20,'','','','');
}

/**
 * INCOMPLETA
 * Fechar avaliação, mas antes verifica todas as pendências
 * @return void(0)
 */
function alteraAvaliacao($status) {
    
    if (pendenciaAvaliacao()) {
        
        monstraObrasPendentesAvaliacao();
        ?>
        <script type="text/javascript">
            notice("Obras pendentes de avaliação");
        </script>
        <?
    }
    else {
        
        if (pendenciaParecer()) {
            
            mostraObrasPendentesParecer();
            ?>
            <script type="text/javascript">
                notice("Obras pendentes de parecer");
            </script>
            <?
        } else {
            ?>
            <script type="text/javascript">
                $(".modal-dialog").dialog("close");
            </script>
            <?php
            $mensagem = ($status == 'F') ? 'Avaliação Finalizada' : 'Avaliação Habilitada';
            abreFechaAvaliacaoObras($status);
            alertlocation(array(
                'alert' => $mensagem,
                'location' => 'pnbe.php?modulo=principal/avaliacao&acao=A'
            ));
        }
    }
    exit;
}

/**
 * Altera status da avaliação e parecer para aberto[A] ou fechado[F]
 * @global object $db
 * @param string $status
 * @return boolean
 */
function abreFechaAvaliacaoObras($status = 'F') {
    
    global $db;
    
    $strStatusAvaid = "UPDATE pnbe.obra o SET obrfechaavalia='{$status}' WHERE obrid > 0";
    $db->executar($strStatusAvaid);
    return $db->commit();
}

/**
 * Verifica se obras estao habilitadas para avaliacao e parecer
 * @global object $db
 * @return int
 */
function avaliacaoAberta() {
    
    global $db;
    //$strSqlVerify = "SELECT COUNT(obrid) FROM pnbe.obra WHERE obrano = '{$_SESSION['exercicio']}' and obrfechaavalia='A'";
   
    $strSqlVerify = "SELECT COUNT(obrid) FROM pnbe.obra WHERE obrano = '{$_SESSION['exercicio']}' and (obrfechaavalia is null or obrfechaavalia = 'A') and sitid = ".OBRAS_SELECIONADAS_PREANALISE;
    $retornoQuery = $db->pegaUm($strSqlVerify);

   if ($retornoQuery <= 0  || !$retornoQuery ){
    $retorno = 's';
    }else{
        $retorno = 'c';
    }
        
    return $retorno;
}

/**
 * Retona o botão de acordo com o status
 * @return string
 */
function retornaDadosBtnAvaid() {
    
    $btn = array();
    $valorAva = avaliacaoAberta();

    if ($valorAva == 'c') {
        $btn['class'] = 'enable';
        $btn['text'] = 'Fechar Avaliação';
    } else {
        $btn['class'] = 'disable';
        $btn['text'] = 'Avaliação Finalizada';
    }
    
    return $btn;
}

/**
 * Carrega scripts estaticos, js, css, etc...
 * @return String (HTML)
 */
function loadStaticScripts() {
    
    $files = array(
        'js' => array(
            '/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js',
            '/pnbe/js/jquery.easing.min.js',
            '/pnbe/jquery-ui-1.9.2.custom/js/jquery-ui-1.9.2.custom.min.js',
            '/pnbe/js/pnbe.js'
        ),
        'css' => array(
            'http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.9/themes/base/jquery-ui.css',
            '/pnbe/css/pnbe.css'
        )
    );
    
    $markup = array(
        'js' => '<script src="%s" type="text/javascript"></script>', 
        'css' => '<link rel="stylesheet" type="text/css" href="%s"/>'
    );
    
    $output = '';
    foreach ($files['js'] as $file) {
        $output.= sprintf($markup['js'], $file);
    }
    
    foreach ($files['css'] as $file) {
        $output.= sprintf($markup['css'], $file);
    }
    
    echo $output.'<div class="modal-dialog" id="modalDialog"></div>';
}

/**
 * Joga o arquivo para o browser fazer download
 * @global object $db
 * @param int $fileID
 */
function download($fileID) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "obra", NULL, "pnbe" );
    $file->getDownloadArquivo( $fileID );
}

# - salvarDadosCadastroObra: SALVA, ALTERA e INATIVA DADOS DA OBRA - TELA DE CADASTRO DE OBRA.
function salvarObras($dados){
    global $db;

    $obrid                  = $dados['obrid'];
    $obrcodigo              = trim($dados['obrcodigo']);
    $obrtitulo              = trim($dados['obrtitulo']);
    $obrautor               = trim($dados['obrautor']);
    $obrano                 = trim($dados['obrano']);
    $catid                  = $dados['catid'];
    $ediid                  = $dados['ediid'];
    $del                    = $dados['del'];
    
    if( $obrid == '' && $del == ''){
        $sql = "INSERT INTO pnbe.obra( 
                    obrcodigo, 
                    obrtitulo, 
                    catid, 
                    ediid, 
                    obrautor, 
                    obrano, 
                    obrstatus,
                    sitid)
                VALUES ( 
                    '{$obrcodigo}', 
                    '{$obrtitulo}', 
                    $catid, 
                    $ediid,
                    '{$obrautor}', 
                    $obrano, 
                    'A',
                    '1')
                returning obrid";
    }elseif($obrid != '' && $del == 1){
        $sql = "UPDATE pnbe.obra SET 
                    obrstatus = 'I' 
                WHERE obrid = $obrid returning obrid";
    }else{
        $sql = "UPDATE pnbe.obra SET 
                    obrcodigo = '{$obrcodigo}', 
                    obrtitulo = '{$obrtitulo}', 
                    catid = $catid, 
                    ediid = $ediid, 
                    obrautor = '{$obrautor}', 
                    obrano = $obrano,
                    obrstatus = 'A' 
                WHERE obrid = $obrid returning obrid";
    }
    $dado = $db->pegaLinha($sql);
    if( $dado > 0 ){
        $db->commit();
            $_SESSION['livro']['obrid'] = $dado;
            $db->sucesso('sistema/tabelaapoio/cadastra_obra&acao=A', '&cprid='.$obrid);
    }else{
        $db->insucesso('Não foi possível gravar o Dados, tente novamente mais tarde!', '', 'inicio&acao=C');
    }
}