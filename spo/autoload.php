<?php
/**
 * Implementação do autload das classes dos módulos SPO.
 *
 * $Id: autoload.php 102340 2015-09-11 13:12:31Z maykelbraz $
 */

/*
 * Gravar uma linha no Aviso ao Usuário
 * Parâmetros: sisid, usucpf, mensagem, url
 */
function cadastrarAvisoUsuario($params){
    global $db;
    $sql = " INSERT INTO public.avisousuario (sisid, usucpf, avbmsg, avburl, avbstatus) VALUES ({$params['sisid']},'{$params['usucpf']}','{$params['mensagem']}','{$params['url']}', 'A')";
    $db->executar($sql);
	$db->commit();
    return true;
}

/**
 * Função de identificação das classes do SPO
 *
 * @param string $nomeClasse Nome da classe não inclusa, instanciada.
 */
function spo_autoload($nomeClasse)
{
    // -- root do sistema
    $path = APPRAIZ;
    $ds = DIRECTORY_SEPARATOR;

    $componentes = explode('_', $nomeClasse);

    // -- Nome do arquivo de implementação da classe
    $arquivo = array_pop($componentes);

    // -- transformando todos os componentes para minusculo
    $componentes = array_map('strtolower', $componentes);

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

    $modulo = array_shift($componentes);
    switch ($modulo) {
        case 'simec':
            $path .= "includes{$ds}library{$ds}simec{$ds}";
            break;
        default:
            $path .= "{$modulo}{$ds}classes{$ds}";
    }

    // -- Processando os demais componentes do nome das classes
    foreach ($componentes as $_path) {
        $path .= "{$_path}{$ds}";
    }

    $path .= $arquivo;

    if (is_file("{$path}.class.inc")) {
        $path .= '.class.inc';
    } elseif (is_file("{$path}.inc")) {
        $path .= '.inc';
    } elseif (is_file("{$path}.php")) {
        $path .= '.php';
    } else {
        $path = '';
    }

    if (!empty($path)) {
        /**
         * Inclusão da classe solicitada.
         */
        require_once($path);
    }
}

# definindo função do autoload
spl_autoload_register('spo_autoload');

//initAutoload();


/**
 * Funções comuns aos sistemas SPO.
 * @see funcoesspo.php
 */
require_once APPRAIZ . 'includes/funcoesspo.php';
