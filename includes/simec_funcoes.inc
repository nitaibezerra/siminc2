<?php
/*
 * simec_htmlentities - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_htmlentities($string, $flags = null, $encoding = 'ISO-8859-1', $double_encode = null){
    return htmlentities($string, $flags, $encoding, $double_encode);
}

/*
 * simec_htmlspecialchars - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_htmlspecialchars($string, $flags = ENT_COMPAT, $charset = 'ISO-8859-1'){
    return htmlspecialchars($string, $flags, $charset);
}

/*
 * simec_html_entity_decode - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_html_entity_decode($string, $quote_style = ENT_COMPAT, $charset = 'ISO-8859-1'){
    return html_entity_decode($string, $quote_style, $charset );
}

/*
 * simec_json_encode - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_json_encode($value){
    return json_encode(simec_utf8_encode_recursive($value));
}

/*
 * simec_json_decode - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_json_decode($value){
    return json_decode(simec_utf8_decode_recursive($value));
}

/*
 * simec_get_class - Função utilizada para corrigir problema de padrão do get_class
 */
function simec_get_class($value){
    if( is_object($value) ){
        return get_class($value);
    } else {
        return false;
    }
}

function simec_email( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='', $arquivos = Array(), $param = Array() ){
    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
    $mensagem = new PHPMailer();
    global $db;
    $mensagem->persistencia = $db;
//	$mensagem->Host         = "localhost";
    $mensagem->Host         = $_SESSION['config_smtp'];
    $mensagem->Mailer       = "smtp";

    /* Regras definidas pelo Cristiano (11/11/2008)*/
    //$mensagem->From = $remetente['email'];

    /* Regras redefinidas pelo Cristiano (08/10/2009)*/
    $mensagem->From = $_SESSION['email_sistema'];

    if(count($remetente)==2) {
        $mensagem->AddReplyTo($remetente['email'],$remetente['nome']);
        $mensagem->FromName = $remetente['nome'];
    } elseif($remetente != '') {
        $mensagem->FromName = is_array($remetente) ? $remetente['nome'] : "Administrador ". SIGLA_SISTEMA;
    } else {
        $mensagem->FromName = "Administrador ". SIGLA_SISTEMA;
    }
    /* FIM regras*/

    if( is_array( $destinatario ) ){
        foreach( $destinatario as $email ){
            if(is_array($email)){
                if(isset($email['usuemail']) && isset($email['usunome'])){
                    $mensagem->AddAddress( $email['usuemail'], $email['usunome']);
                } else {
                    $mensagem->AddAddress($email);
                }
            } else {
                $mensagem->AddAddress($email);
            }
        }
    }
    else{
        $mensagem->AddAddress( $destinatario );
    }

    /*
     * Atualizado pelo Alexandre Dourado
     * recebe um array de email
     */
    if(is_array($cc) || count($cc) > 1) {
        foreach($cc as $email) {
            $mensagem->AddCC( $email );
        }
    } else {
        $mensagem->AddCC( $cc );
    }
    /*
     * FIM Atualizado pelo Alexandre Dourado
     */

    /*
     * Atualizado pelo Ruy Ferreira
     * copia oculta
     */
    if(is_array($cco)){
        foreach($cco as $valueCco){
            $valueCco = trim( $valueCco );
            if( $valueCco ) $mensagem->AddBCC( $valueCco );
        }
    } else {
        $valueCco = trim( $cco );
        if( $valueCco ) $mensagem->AddBCC( $valueCco );
    }

    /*
     * FIM Atualizado pelo Ruy Ferreira
     */



    # Atualizado pelo Wesley Romualdo
    //anexa os arquivos
    foreach ( $arquivos as $arquivo ){
        if ( !file_exists( $arquivo['arquivo'] ) ) {
            continue;
        }
        $arqtipo = isset($arquivo['arqtipo']) ? $arquivo['arqtipo'] : 'application/octet-stream';
        $mensagem->AddAttachment($arquivo['arquivo'], basename($arquivo['nome']) , 'base64', $arqtipo);
    }
    # Fim Atualizado pelo Wesley Romualdo

    # Configuração extra do envio de email
    foreach( $param as $attr => $value ){
        switch ($value){
            case 'CustomHeader':
                foreach($value as $val){
                    $mensagem->AddCustomHeader($val);
                }
                break;
            default:
                $mensagem->$attr = $value;
                break;
        }
    }
    # FIM Configuração extra do envio de email

    $mensagem->Subject = $assunto;
    $mensagem->Body = $conteudo;
    $mensagem->IsHTML( true );

    # Verifica se o SMTP está configurado pra enviar e-mail. Se estiver, envia.
    if($_SESSION['config_smtp']){
        return $mensagem->Send();
    } else {
        return FALSE;
    }
}

/*
 * simec_number_format - Função utilizada para formatar numeros
 */
function simec_number_format($number, $decimals = 2, $dec_point = ',', $thousands_sep = '.') {
    if (!is_numeric($number)) {
        return '0,00';
    }

    $decimals = is_numeric($decimals) ? $decimals : 0;
    return number_format($number, $decimals, $dec_point, $thousands_sep);
}

/*
 * simec_utf8_decode_recursive - Função utilizada para formatar decode recursivo
 */
function simec_utf8_decode_recursive($array)
{
    $utf8_array = array();

    if (is_array($array))
    {
        foreach ($array as $key => $val)
        {
            if (is_array($val))
                $utf8_array[$key] = simec_utf8_decode_recursive($val);
            else if (is_object($val))
                $utf8_array[$key] = $val;
            else
                $utf8_array[$key] = utf8_decode($val);
        }
    }
    else
    {
        $utf8_array = $array;
    }

    return $utf8_array;
}

/*
 * simec_utf8_encode_recursive - Função utilizada para formatar encode recursivo
 */
function simec_utf8_encode_recursive($array)
{
    $utf8_array = array();

    if (is_array($array))
    {
        foreach ($array as $key => $val)
        {
            if (is_array($val))
                $utf8_array[$key] = simec_utf8_encode_recursive($val);
            else if (is_object($val) || is_numeric($val))
                $utf8_array[$key] = $val;
            else
                $utf8_array[$key] = utf8_encode($val);
        }
    }
    else
    {
        $utf8_array = $array;
    }

    return $utf8_array;
}

/*
 * simec_trim - Função utilizada para remover caracteres de uma string
 */
function simec_trim($string, $charList = ' ')
{
    if (is_array($string))
    {
        $result = array();

        foreach ($string as $key => $val)
        {
            if (is_array($val))
                $result[$key] = simec_trim($val, $charList);
            else if (is_object($val) || is_numeric($val))
                $result = $string;
            else {
                $result[$key] = trim($val, $charList);
            }
        }

        return $result;
    }
    else
    {
        return trim($string, $charList);
    }
}

/*
 * simec_strip_tags - Função utilizada para remover caracteres especiais de uma string
 */
function simec_strip_tags($string, $allowableTags = null)
{
    if (is_string($string))
        return strip_tags($string, $allowableTags);

    return $string;
}

/*
 * simec_addslashes - Função utilizada para adicionar escape nas aspas
 */
function simec_addslashes($var)
{
    return is_array($var) ? array_map("simec_addslashes", $var) : addslashes(stripslashes($var));
}

/*
 * simec_stripslashes - Função utilizada para retirar escape nas aspas
 */
function simec_stripslashes($var)
{
    return is_array($var) ? array_map("simec_stripslashes", $var) : stripslashes($var);
}

/*
 * simec_magic_quotes - Simula automaticamente o magic_quotes
 */
function simec_magic_quotes()
{
    $_GET           = simec_addslashes($_GET);
    $_POST          = simec_addslashes($_POST);
    $_COOKIE        = simec_addslashes($_COOKIE);
    $_REQUEST       = simec_addslashes($_REQUEST);
}

/*
 * simec_gravar_estatistica - Grava as estatísticas do SIMEC
 */
function simec_gravar_estatistica($mnuid = null, $usucpf = null, $esttempoexec = null, $estsession = null, $sisid = null, $estmemusa = null, $estip = null)
{
    global $db;
    global $Tinicio;

    $mnuid = $mnuid ? $mnuid : $_SESSION['mnuid'];
    $usucpf = $usucpf ? $usucpf : $_SESSION['usucpforigem'];
    $esttempoexec = $esttempoexec ? $esttempoexec : (getmicrotime() - $Tinicio);
    $estsession = $estsession ? $estsession : session_id();
    $sisid = $sisid ? $sisid : $_SESSION['sisid'];
    $estmemusa = $estmemusa ? $estmemusa : number_format(memory_get_usage()/(1024*1024),2,'.','');
    $estip = $estip ? $estip : $_SERVER["REMOTE_ADDR"];

    if($mnuid && $usucpf && $esttempoexec && $estsession && $sisid && $estmemusa && $estip){
        //Loga estatistica
        $tabelaEstatistica = 'estatistica.estatistica_' . date('Y_m');

        $sql = "Insert into {$tabelaEstatistica} (mnuid, usucpf, esttempoexec, estsession, sisid, estmemusa, estip)
                                           VALUES(" . $mnuid . ", '" . $usucpf . "', " . $esttempoexec . ", '" . $estsession . "', " . $sisid . ", '".$estmemusa."', '".$estip."')";
        $db->executar($sql,false);
        $db->commit();
    }
}

/*
 * simec_preparar_array - Prepara array para select
 */
function simec_preparar_array($dados = null, $where = null, $order = null)
{
    $array = array();

    $dados = $dados ? $dados : [];
    foreach ($dados as $linha) {
        $array[$linha['codigo']] = $linha['descricao'];
    }

    return $array;
}

function simec_redirecionar($url, $tipo = null, $mensagem = null)
{
    if($tipo || $mensagem){
        $mensagem = $mensagem ? $mensagem : ($tipo == 'success' ? 'Operação realizada com sucesso' : 'Ocorreu um erro ao executar operação');
        $_SESSION['mensagem_carregar'] = array('type'=>$tipo, 'text'=>$mensagem);
    }

    ob_clean();
    header("Location: {$url}");
    die;
}

function simec_multi_in_array($value, $array)
{
    foreach ($array AS $item)
    {
        if (!is_array($item))
        {
            if ($item == $value)
            {
                return true;
            }
            continue;
        }

        if (in_array($value, $item))
        {
            return true;
        }
        else if (simec_multi_in_array($value, $item))
        {
            return true;
        }
    }

    return false;
}


/**
 * Função de identificação das classes do SPO
 *
 * @param string $nomeClasse Nome da classe não inclusa, instanciada.
 */
function simec_autoload($nomeClasse)
{
    // -- root do sistema
    $path = $pathZendSimec = APPRAIZ;
    $ds = DIRECTORY_SEPARATOR;

    $componentes = explode('_', $nomeClasse);
    // -- Nome do arquivo de implementação da classe
    $arquivo = array_pop($componentes);

    // -- Modelo, classe de abstração de conexões e classes
    if (false !== strstr($nomeClasse, 'Spo_Ws_Sof_')) {
        require_once "{$path}{$ds}spo{$ds}ws{$ds}sof{$ds}{$arquivo}.php";
        return;
    }

    switch ($arquivo) {
        case 'Modelo':
            require_once("{$path}{$ds}includes{$ds}classes{$ds}Modelo.class.inc");
            return;
        case 'FilesSimec':
            require_once("{$path}{$ds}includes{$ds}classes{$ds}fileSimec.class.inc");
            return;
        case 'Grafico':
            require_once("{$path}{$ds}includes{$ds}library{$ds}simec{$ds}Grafico.php");
            return;
        case 'FlashMessage':
            require_once("{$path}{$ds}includes{$ds}library{$ds}simec{$ds}Helper{$ds}FlashMessage.php");
            return;
        case 'DML':
            require_once "{$path}{$ds}includes{$ds}library{$ds}simec{$ds}DB{$ds}{$arquivo}.php";
            return;
    }

    if(empty($componentes)){
        $componentes = array($_SESSION['sisdiretorio']);
    }

    $modulo = strtolower(array_shift($componentes));
    switch ($modulo) {
        case 'simec':
            $path .= "includes{$ds}library{$ds}simec{$ds}";
            break;
        case 'zend':
            $path .= "zimec{$ds}library{$ds}Zend{$ds}";
            break;
        default:
            $path .= "{$modulo}{$ds}classes{$ds}";
    }

    $pathZendSimec .= "zimec{$ds}library{$ds}Simec{$ds}";
    // -- Processando os demais componentes do nome das classes
    foreach ($componentes as $_path) {
        $path .= "{$_path}{$ds}";
        $pathZendSimec .= "{$_path}{$ds}";
    }

    $caminhoPadrao = $path;
    $caminhoModel  = $path . 'model' . $ds;
    $caminhoModelo  = $path . 'modelo' . $ds;
    $caminhoController  = $path . 'controller' . $ds;

    $paths = array($caminhoPadrao, $caminhoModel, $caminhoModelo, $caminhoController, $pathZendSimec);
    $extensoes = array('.class.inc', '.inc', '.php');
    foreach ($paths as $path) {
        foreach ($extensoes as $extensao) {
            $file = $path . $arquivo . $extensao;
            if (is_file($file)) {
                require_once($file);
                return;
            } elseif (false !== strpos($file, "{$ds}Model{$ds}") && is_file(str_replace("{$ds}Model{$ds}", "{$ds}model{$ds}", $file))) {
                // -- tratamento "model" de diretórios
                require_once(str_replace("{$ds}Model{$ds}", "{$ds}model{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}Controller{$ds}") && is_file(str_replace("{$ds}Controller{$ds}", "{$ds}controller{$ds}", $file))) {
                // -- tratamento "model" de diretórios
                require_once(str_replace("{$ds}Controller{$ds}", "{$ds}controller{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}Service{$ds}") && is_file(str_replace("{$ds}Service{$ds}", "{$ds}service{$ds}", $file))) {
                // -- tratamento "service" de diretórios
                require_once(str_replace("{$ds}Service{$ds}", "{$ds}service{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}View{$ds}") && is_file(str_replace("{$ds}View{$ds}", "{$ds}view{$ds}", $file))) {
                // -- tratamento "service" de diretórios
                require_once(str_replace("{$ds}View{$ds}", "{$ds}view{$ds}", $file));
                return;
            }
        }
    }
}

function initAutoload()
{
    $funcs = spl_autoload_functions();

    # definindo função do autoload
    spl_autoload_register('simec_autoload');

    if(is_array($funcs) && in_array('__autoload', $funcs)){
        spl_autoload_register('__autoload');
    }
    if(is_array($funcs) && in_array('spo_autoload', $funcs)){
        spl_autoload_register('spo_autoload');
    }
}

/**
 * Busca Sub-Unidades vinculadas ao usuário.
 *
 * @return array
 */
function buscarSubUnidadeUsuario(stdClass $filtros){
    global $db;
    $listaSubUnidadeUsuario = array();
    $sql = "
        SELECT DISTINCT
            suo.suocod
        FROM {$_SESSION['sisdiretorio']}.usuarioresponsabilidade rpu
            JOIN public.vw_subunidadeorcamentaria suo ON rpu.ungcod = suo.suocod
        WHERE
            rpu.rpustatus = 'A'
            AND rpu.usucpf = '". pg_escape_string($filtros->usucpf). "'
    ";
//ver($sql,d);
    $resultado = $db->carregar($sql);
    if($resultado){
        foreach($resultado as $contador => $subUnidade){
            $listaSubUnidadeUsuario[] = $subUnidade['suocod'];
        }
    }

    return $listaSubUnidadeUsuario;
}

/**
 * Função para gerar funções em javascript
 * @param unknown $sisid
 * @param unknown $sisDir
 * @param unknown $usucpf
 * @param unknown $dados
 * @return string
 */
function gerFuncResp($sisid, $sisDir, $usucpf, $dados = array()) {
    $script = sprintf ( "function popresp_%s( pflcod, tprsigla ) {switch( tprsigla ){", $sisid );

    foreach ( $dados as $dado ) {
        $script .= sprintf ( " case '%s':
                                   abreresp%s = window.open(
                                   '../%s/geral/cadastro_responsabilidade_%s.php%spflcod='+pflcod+'&usucpf=%s',
                                   'popresp_%s',
                                   'menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=500,height=540');
                                   break;", $dado ['tprsigla'], $sisid, $sisDir, $dado ['tprurl'], (strpos ( $dado ['tprurl'], '?' ) !== false ? '&' : '?'), $usucpf, $sisid );
    }

    $script .= "\n }\n";
    $script .= sprintf ( " abreresp%s.focus();\n", $sisid );
    $script .= " }\n\n";
    return $script;

}

function carregaDadosUsuario($usucpf){
    global $db;

    $sql = sprintf ("SELECT
                        u.*,
                        e.entid
                    FROM
                        seguranca.usuario u
                    LEFT JOIN
                        entidade.entidade e ON
                        u.entid = e.entid
                    WHERE
                        usucpf = '%s'", $usucpf );

    $usuario = $db->pegaLinha ( $sql );

    if (! $usuario) {
        $_REQUEST ['acao'] = "A";
        if ($_SESSION ['sisid'] == 147) { // obras 2
            $db->insucesso ( "Usuário Não Encontrado", "&acao=A", "sistema2/usuario2/consusuario" );
        } else {
            $db->insucesso ( "Usuário Não Encontrado", "&acao=A", "sistema/usuario/consusuario" );
        }
    }
    return $usuario;
}

function alteraDadosUsuario($info){
    global $db;
    // Só quem pode alterar senha ou email (em caso de super-usuário) é a própria pessoa
    if ($_SESSION ['usucpforigem'] != $info ['usucpf'] || $_SESSION ['usucpforigem'] != $_SESSION ['usucpf']) {
        $usucpf = $info['usucpf'];
        $sql = "SELECT * FROM seguranca.usuario WHERE usucpf = '{$usucpf}'";
        $dadosUsuario = $db->pegaLinha ( $sql );

        $sql = "SELECT 
                    count(*) as qtd
                FROM 
                    seguranca.perfil p
                INNER JOIN 
                    seguranca.perfilusuario pu on p.pflcod = pu.pflcod
                WHERE 
                    p.pflsuperuser = 't'
                AND 
                    usucpf = '{$info['usucpf']}'";
        $qtdDestino = $db->pegaUm ( $sql );

        if ($qtdDestino) {
            if ($info ['senha']) {
                enviarAlteracaoSuperUser ( $dadosUsuario, 'senha' );
                echo "<script>
                        alert('Você não tem permissão para alterar a senha deste usuário.');
                        window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                      </script>";
                exit ();
            }

            if ($dadosUsuario ['usuemail'] != $info ['usuemail']) {
                $dadosUsuario ['usuemaildestino'] = $info ['usuemail'];
                enviarAlteracaoSuperUser ( $dadosUsuario, 'email' );
                echo "<script>
                        alert('Você não tem permissão para alterar o email deste usuário.');
                        window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                      </script>";
                exit ();
            }
        }
    }
    // $tpocod_banco = $_REQUEST['tpocod'] ? (integer) $_REQUEST['tpocod'] : "null";

    // data de nascimento
    $dataBanco = formata_data_sql ( $info ['usudatanascimento'] );
    $dataBanco = $dataBanco ? "'" . $dataBanco . "'" : "null";

    // Caso tenha entidade
    $entid = isset( $info['entid'] ) && !isset($info['orgao'])  ? $info['entid'] : 'NULL';
    $entid = $entid == 999999 ? 'NULL' : $entid;

    // arrumando problema de slashes excessivos causados pela diretiva magic codes do php
    $orgao = (str_replace ( "\\", "", $info ['orgao'] ));
    $orgao = stripcslashes ( $orgao );
    $orgao = str_replace ( "'", "", $orgao );

    /*
    * Integração com o SSD Atualizando os possiveis novos dados Desenvolvido por Alexandre Dourado
    */
    if (AUTHSSD) {
        // Definindo o local dos certificados
        // define("PER_PATH", "../");
        include_once (APPRAIZ . "www/connector.php");
        $SSDWs = new SSDWsUser ( $tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain );
        // Efetuando a conexão com o servidor (produção/homologação)
        if ($GLOBALS ['USE_PRODUCTION_SERVICES']) {
            $SSDWs->useProductionSSDServices ();
        } else {
            $SSDWs->useHomologationSSDServices ();
        }

        $SSD_tipo_pessoa = @utf8_encode ( "F" );
        $SSD_nome = @utf8_encode ( $_POST ["usunome"] );
        $SSD_cpf = @utf8_encode ( $info['usuario']['usucpf'] );
        $SSD_data_nascimento = @utf8_encode ( formata_data_sql ( $_POST ["usudatanascimento"] ) );

        if ($_POST ['usuemailconfssd'] != $_POST ["usuemail"]) {
            $SSD_email = @utf8_encode ( $_POST ["usuemail"] );
        }

        $SSD_ddd_telefone = @utf8_encode ( $_POST ["usufoneddd"] );
        $SSD_telefone = @utf8_encode ( $_POST ["usufonenum"] );

        $userInfo = "$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" . "$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" . "$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" . "$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||" . "$justificativa||$cpf_responsavel||ssd";

        $resposta = $SSDWs->updateUser ( $userInfo );

        if ($resposta != "true") {
            if ($_SESSION ['sisid'] == 147) { // obras 2
                echo "<script>
                        alert('" . addslashes ( $resposta ["erro"] ) . "');
                        window.location = '?modulo=sistema2/usuario2/consusuario&acao=A';
                      </script>";
                exit ();
            } else {
                echo "<script>
                            alert('" . addslashes ( $resposta ["erro"] ) . "');
                            window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                      </script>";
                exit ();
            }
        }
    }
    //moldando objetos
    $usuario = new stdClass();
    $usuario = $info['usuario'];
    /*
    * FIM Integração com o SSD Atualizando os possiveis novos dados Desenvolvido por Alexandre Dourado
    */
    // variável permissão cadastro_usuario_geral_bootstrap
    $info ['carid'] = ($info ['carid'] != '') ? $info ['carid'] : 'NULL';
    if (!empty($info['permissao'])) {

        $sql = sprintf ( "
            UPDATE seguranca.usuario 
                SET
                usunome           = '" . $_POST ['usunome'] . "',
                usuemail          = '%s',
                usufoneddd        = '%s',
                usufonenum        = '%s',
                usufuncao         = '%s',
                carid             = %s,
                entid             = %s,
                unicod            = '%s',
                regcod            = '%s',
                ungcod            = '%s',
                ususexo           = '%s',
                usudatanascimento =  %s,
                usunomeguerra     = '%s',
                muncod            = '%s',
                orgao             = '%s',
                tpocod            = '%s',
                usudataatualizacao = 'now()'
            WHERE
                usucpf            = '%s'",
            pg_escape_string ( $info['usuemail'] ),
            pg_escape_string ( $info['usufoneddd'] ),
            str_replace ( "\\", "", substr ( $info['usufonenum'], 0, 10 ) ),
            pg_escape_string ( substr ( $info['usufuncao'], 0, 100 ) ),
            pg_escape_string ( $info['carid'] ),
            pg_escape_string ( $entid ),
            pg_escape_string ( $info['unicod'] ),
            pg_escape_string ( $info['regcod'] ),
            pg_escape_string ( $info['ungcod'] ),
            pg_escape_string ( $info['ususexo'] ),
            $dataBanco,
            pg_escape_string ( $info['usunomeguerra'] ),
            pg_escape_string ( $info['muncod'] ),
            str_replace ( "'", "", $orgao ),
            pg_escape_string ( $info['tpocod'] ),
            pg_escape_string ( $usuario->usucpf ) );
    } else {
        $data_atual = $data_atual ? "'" . $data_atual . "'" : "null";
        // atualiza dados gerais do usuário
        $sql = sprintf ( "
            UPDATE seguranca.usuario 
                SET
                usuemail = '%s',
                usufoneddd = '%s',
                usufonenum = '%s',
                usufuncao = '%s',
                carid = '%s',
                entid = '%s',
                unicod = '%s',
                regcod = '%s',
                ungcod = '%s',
                ususexo = '%s',
                usudatanascimento = %s,
                usunomeguerra = '%s',
                muncod = '%s',
                orgao = '%s',
                tpocod = '%s',
                usudataatualizacao = %s
            WHERE
                usucpf = '%s'",
            pg_escape_string ( $info['usuemail'] ),
            pg_escape_string ( $info['usufoneddd'] ),
            str_replace ( "\\", "", substr ( $info['usufonenum'], 0, 10 ) ),
            pg_escape_string ( $info['usufuncao'] ),
            pg_escape_string ( $info['carid'] ),
            pg_escape_string ( $entid ),
            pg_escape_string ( $info['unicod'] ),
            pg_escape_string ( $info['regcod'] ),
            pg_escape_string ( $info['ungcod'] ),
            pg_escape_string ( $info['ususexo'] ),
            $dataBanco,
            pg_escape_string ( $info['usunomeguerra'] ),
            pg_escape_string ( $info['muncod'] ),
            str_replace ( "'", "", $orgao ),
            pg_escape_string ( $info['tpocod'] ),
            $data_atual,
            pg_escape_string ( $info['usucpf'] )
        );
    }
    $booAlterar = $db->executar( $sql );
    // altera a senha do usuário com o valor padrão
    if ($info['senha']) {
        /*
        * Integração com o SSD Atualizando nova senha por padrão Desenvolvido por Alexandre Dourado
        */
        if (AUTHSSD) {
            // Definindo o local dos certificados
            // define("PER_PATH", "../");

            include_once (APPRAIZ . "www/connector.php");
            $SSDWs = new SSDWsUser ( $tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain );
            // Efetuando a conexão com o servidor (produção/homologação)
            if ($GLOBALS ['USE_PRODUCTION_SERVICES']) {
                $SSDWs->useProductionSSDServices ();
            } else {
                $SSDWs->useHomologationSSDServices ();
            }
            $cpfOrCnpj = $usuario->usucpf;
            $oldPassword = base64_encode ( md5_decrypt_senha ( $usuario->ususenha, '' ) );
            $newPassword = base64_encode ( SENHA_PADRAO );
            $resposta = $SSDWs->changeUserPasswordByCPFOrCNPJ ( $cpfOrCnpj, $oldPassword, $newPassword );
            if ($resposta != "true") {
                if ($_SESSION ['sisid'] == 147) { // obras 2
                    echo "<script>
                            alert('" . addslashes ( $resposta ["erro"] ) . "');
                            window.location = '?modulo=sistema2/usuario2/consusuario&acao=A';
                            </script>";
                    exit ();
                } else {
                    echo "<script>
                            alert('" . addslashes ( $resposta ["erro"] ) . "');
                            window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                            </script>";
                    exit ();
                }
            }
        }
        /*
         * FIM Integração com o SSD Atualizando nova senha por padrão Desenvolvido por Alexandre Dourado
        */
        $sql = sprintf ( "UPDATE
                            seguranca.usuario
                        SET
                            ususenha         = '%s',
                            usuchaveativacao = 'f'
                        WHERE
                            usucpf = '%s'", md5_encrypt_senha ( SENHA_PADRAO, '' ), $_REQUEST ['usucpf'] );

        $db->executar ( $sql );
    }
    // aplica as alterações de status nos sistemas
    foreach ( ( array ) $_REQUEST ['status'] as $sisid => $suscod ) {
        $sql = sprintf ( "SELECT us.* FROM seguranca.usuario_sistema us WHERE sisid = %d AND usucpf = '%s'", $sisid, $_REQUEST['usucpf'] );
        $usuariosistema = ( object ) $db->pegaLinha ( $sql );
        if (! $usuariosistema->sisid) {
            $sql = sprintf ( "INSERT INTO seguranca.usuario_sistema ( sisid, usucpf ) VALUES ( %d, '%s' )", $sisid, $_REQUEST['usucpf'] );
            $db->executar ( $sql );
        }

        if ($usuariosistema->suscod != $suscod) {
            $justificativa = $_REQUEST ['justificativa'] [$sisid];
            $db->alterar_status_usuario ( $_REQUEST['usucpf'], $suscod, $justificativa, $sisid );
            $email_aprovacao = $usuariosistema->suscod == 'P' && $suscod == 'A' ? true : $email_aprovacao;
        }
    }

    // executa rotina para alteração do status geral no sistema
    if ($_SESSION ['sisid'] == 4 /*in_array( 'geral', $configuracao )*/ ) {
        if ($usuario->suscod != $_REQUEST ['suscod']) {
            $db->alterar_status_usuario ( $_REQUEST['usucpf'], $_REQUEST ['suscod'], $_REQUEST ['htudsc'] );
            $email_aprovacao = $usuario->suscod == 'P' && $_REQUEST ['suscod'] == 'A';
        }
    }

    // envia o email de confirmação caso a conta seja aprovada
    if ($email_aprovacao) {
        $remetente = array (
            "nome" => $_SESSION ['usunome'],
            "email" => $_SESSION ['usuemail']
        );
        $destinatario = $_REQUEST ['usuemail'];
        $assunto = "Aprovação do Cadastro no ". SIGLA_SISTEMA;
        $conteudo = "
                <br />
                <span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
                <br />
                <span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
                <br />
                <br />
            ";
        $conteudo .= sprintf ( "%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>", $_REQUEST ['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.', $_REQUEST ['usunome'], md5_decrypt_senha ( $usuario->ususenha, '' ) );
        $conteudo .= "
                <br />
                <a style='color: #0000ff;' href='". URL_SISTEMA. "' title='".NOME_SISTEMA ."'>". URL_SISTEMA. "</a>
                <br />
                <br />
            ";
        $corpoEmailV3 = '<p>'. $conteudo. '</p>';
        include APPRAIZ . "www/email-template-novo-bootstrap-v3.php";
//ver($textoEmailV3, d);
        enviar_email ( $remetente, $destinatario, $assunto, $textoEmailV3 );
    }
    // cadastra os perfils selecionados
    if (is_array($_REQUEST ['pflcod'])) {
        foreach ($_REQUEST ['pflcod'] as $sisid => $perfis) {
            if ($perfis <> '') {
                //varrendo se já existe perfil cadastrado para o cpf informado
                $sqlPerfilAntigo = sprintf ( "SELECT * FROM seguranca.perfilusuario
                                      WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )", $_REQUEST ['usucpf'], $sisid );
                $perfisAntigos = $db->carregar ( $sqlPerfilAntigo );
                $aPerfisAntigos = array ();
                //armazenando os perfis antigos
                if ($perfisAntigos && is_array ( $perfisAntigos )) {
                    foreach ( $perfisAntigos as $perfilAntigo ) {
                        $aPerfisAntigos [] = $perfilAntigo ['pflcod'];
                    }
                }

                // deleta os perfis encontrados, mas já estão salvos na variável $aPerfisAntigos
                $sql = sprintf ( "DELETE FROM seguranca.perfilusuario
                                      WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )", $_REQUEST ['usucpf'], $sisid );
                $db->executar ( $sql );
                removerPerfisSlaves ( $_REQUEST ['usucpf'], $sisid );

                /**
                 * * REGRA DO ENEM **
                 */
                if ($email_aprovacao && $_SESSION ['sisid'] == '24') {
                    $sql = "SELECT fun.funid FROM entidade.entidade ent
                                INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.fuestatus = 'A'
                                INNER JOIN entidade.funcao fun ON fun.funid = fue.funid AND fun.funstatus = 'A'
                                WHERE ent.entstatus = 'A'
                                AND ent.entnumcpfcnpj = '" . $_REQUEST ['usucpf'] . "'";
                    $funid_enem = $db->carregarColuna ( $sql );

                    if ($funid_enem) {
                        foreach ( $funid_enem as $funid ) {
                            /**
                             * * executor **
                             */
                            if ($funid == 83 && ! in_array ( "518", $perfis )) {
                                $sql = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 518 )", $_REQUEST ['usucpf'] );
                                $db->executar ( $sql );
                            }
                            /**
                             * * validador **
                             */
                            if ($funid == 84 && ! in_array ( "519", $perfis )) {
                                $sql = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 519 )", $_REQUEST ['usucpf'] );
                                $db->executar ( $sql );
                            }
                            /**
                             * * certificador **
                             */
                            if ($funid == 85 && ! in_array ( 520, $perfis )) {
                                $sql = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 520 )", $_REQUEST ['usucpf'] );
                                $db->executar ( $sql );
                            }
                        }
                    }//fim if($funid_enem)
                }//fim if($email_aprovacao && $_SESSION ['sisid'] == '24')

                //tratando os perfis recuperados
                $perfis = is_array( $perfis ) ? $perfis : array();
                foreach ( $perfis as $pflcod ) {
                    if (empty ( $pflcod )) {
                        continue;
                    }
                    // verifica se existe os perfis para o cpf
                    $sqlPerfilExiste = "select usucpf from seguranca.perfilusuario where usucpf = '{$_REQUEST ['usucpf']}' and pflcod = {$pflcod}";
                    $existePerfis    = $db->pegaUm ( $sqlPerfilExiste );

                    if ( !$existePerfis ) {
                        // inclui os perfis
                        $sqlInsert = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )", $_REQUEST ['usucpf'], $pflcod );
                        $db->executar ( $sqlInsert );

                        $sql = "SELECT pflcod FROM perfil WHERE pflcod = $pflcod AND pflsuperuser = 't' ";
                        $perfilSuperUser = $db->pegaUm ( $sql );

                        if ($_SESSION ['ambiente'] == 'Ambiente de Produção' && $perfilSuperUser && ! in_array ( $perfilSuperUser, $aPerfisAntigos )) {
                            enviarEmailSuperUser ( $_REQUEST ['usucpf'], $pflcod );
                        }
                    }
                    inserirPerfisSlaves ( $_REQUEST ['usucpf'], $pflcod );
                    atualizarResponsabilidadesSlaves ( $_REQUEST ['usucpf'], $pflcod );
                }

                // INATIVA AS RESPONSABILIDADES (ENTIDADE,ESTADOS,MUNICIPIOS)
                if ($sisid != 4) { // 4=Módulo de Segurança
                    // pega o nome do sistema tabela
                    $sql = sprintf ( "SELECT s.* FROM seguranca.sistema s WHERE sisid = %d", $sisid );
                    $sistema = ( object ) $db->pegaLinha ( $sql );
                    $tabela_aux = $sistema->sisdiretorio;
                    if ($sisid == 14)
                        $tabela_aux = "cte";
                    if ($tabela_aux == 'pde') {
                        // 10=Monitoramento do Plano de Desenvolvimento da Educação / 11=gerenciamento projetos
                        // pde
                        // $sisidaux = "10,11";
                        $sisidaux = "select sisid from seguranca.sistema  where sisdiretorio = 'pde'";
                    } elseif ($sisid == 13 || $sisid == 14) {
                        // 13=PAR - plano de metas / 14=Brasilpro
                        // cte
                        $sisidaux = "13,14";
                    } elseif ($sisid == 1 || $sisid == 6) {
                        // 1="PPA-Monitoramento e Avaliação" / 6="Projetos Especiais"
                        // monitora
                        $sisidaux = "1,6";
                    } elseif ($sisid == 2 || $sisid == 5) {
                        // 2="Programação Orçamentária" / 5="PPA-Elaboração e Revisão"
                        // elabrev
                        $sisidaux = "2,5";
                    } else {
                        $sisidaux = $sisid;
                    }
                    $sql = "SELECT true FROM pg_tables
                                WHERE schemaname = '" . $tabela_aux . "' 
                                AND tablename  = 'usuarioresponsabilidade'";

                    if ($db->pegaUm ( $sql ) == 't') {
                        $sqlr = "UPDATE " . $tabela_aux . ".usuarioresponsabilidade SET rpustatus='I'
                                        WHERE usucpf='" . $_REQUEST ['usucpf'] . "' and rpustatus='A' and pflcod not in( 
                                              SELECT pu.pflcod from seguranca.perfilusuario pu, seguranca.perfil p 
                                              WHERE pu.pflcod=p.pflcod and p.sisid in (" . $sisidaux . ")
                                              AND pu.usucpf ='" . $_REQUEST ['usucpf']  . "')";
                        $db->executar ( $sqlr );
                    }
                }//fim modulo seguranca
                // FIM INATIVA

                $db->commit ();
                //$parametros = '&usucpf=' . $_REQUEST ['usucpf'];
                die ( '<script>
                                alert(\'Operação realizada com sucesso!\');
                                location.href = window.location;
                               </script>');

            }//fim foreach ($_REQUEST ['pflcod'] as $sisid => $pflcod)
        }
    }//fim function alterarDadosUsuarios
    if($booAlterar){//submetendo dados para alteração.
        $db->commit ();
        //$parametros = '&usucpf=' . $_REQUEST ['usucpf'];
        die ( '<script>
                    alert(\'Operação realizada com sucesso!\');
                    location.href = window.location;
                   </script>');
    }//fim booAlterar
}

function mostraResponsabilidades($responsabilidades = array(), $perfisUsuario, $sistema, $exibirBotao = true){
    global $db;
    $responsabilidades = is_array($responsabilidades) ? $responsabilidades : array();
    $colspan = @count($responsabilidades)
    ?>
    <section class="col-md-10">
    <table class="table table-bordered table-condensed">
        <thead>
        <tr class="active">
            <th rowspan="2" colspan="2">Descrição</th>
            <th colspan="<?=$colspan?>">Responsabilidades</th>
        </tr>
        <tr class="active">
            <?php foreach ($responsabilidades as $responsabilidade):
                echo <<<HTML
                        <th>{$responsabilidade["tprdsc"]}</th>
HTML;
            endforeach; ?>
        </tr>
        </thead>
        <tbody style="background-color:white;">
        <?php
        foreach( $perfisUsuario as $perfil ):
            $sqlResponsabilidadesPerfil = <<<DML
				        SELECT
				            p.*,
				            tr.tprdsc,
				            tr.tprsigla
				        FROM (SELECT * FROM {$sistema->sisdiretorio}.tprperfil
				        WHERE pflcod = '%s') p
				        RIGHT JOIN {$sistema->sisdiretorio}.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
				        WHERE tprsnvisivelperfil = TRUE
				        ORDER BY tr.tprdsc
DML;

            $query = sprintf($sqlResponsabilidadesPerfil, $perfil ["pflcod"]);
            $responsabilidadesPerfil = (array) $db->carregar($query);

            // Esconde a imagem + para perfis sem responsabilidades
            $mostraMais = false;

            foreach($responsabilidadesPerfil as $resPerfil) {
                if ((boolean) $resPerfil ["tprcod"]) {
                    $mostraMais = true;
                    break;
                }
            }
            ?>
            <tr>
                <td width="10px" align="center">
                    <? if ($mostraMais): ?>
                        <a href="Javascript:abreconteudo('../<?=$sistema->sisdiretorio; ?>/geral/cadastro_responsabilidades.php?usucpf=<?=$sistema->usuariosistema->usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">
                            <img src="../imagens/mais.gif" name="+" border="0"	id="img<?=$perfil["pflcod"]?>" />
                        </a>
                    <? endif; ?>
                </td>
                <td><strong><?=$perfil["pfldsc"]?></strong></td>
                <? foreach( $responsabilidadesPerfil as $resPerfil ): ?>
                    <td align="center">
                        <? if ( (boolean) $resPerfil["tprcod"]  && $exibirBotao == true): ?>
                            <button type="button" class="btn btn-primary" name="btnAbrirResp<?=$perfil["pflcod"]?>" onclick="popresp_<?= $sistema->sisid ?>(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')" data-toggle="modal" data-target="#myModal">Atribuir</button>
                        <? else: ?>
                            -
                        <? endif; ?>
                    </td>
                <? endforeach; ?>
            </tr>
            <tr>
                <td style="display: none;" colspan="<?=$colspan+2?>" id="td<?=$perfil["pflcod"]?>"></td>
            </tr>
        <? endforeach; ?>
        </tbody>
    </table>
    </section><?
}
