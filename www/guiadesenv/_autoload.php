<?php

define( 'APPRAIZ_ZEND', APPRAIZ . 'includes/library/Zend/');

define('CLASSES_GERAL', APPRAIZ . "includes/classes/");
define('CLASSES_CONTROLE', APPRAIZ . 'guiadesenv/classes/controllers/');
define('CLASSES_MODELO', APPRAIZ . 'guiadesenv/classes/models/');
define('CLASSES_VISAO', APPRAIZ . 'guiadesenv/classes/views/');
//define('CLASSES_VISAO'  , APPRAIZ . 'includes/classes/view/');
//define('CLASSES_HTML'  , APPRAIZ . 'includes/classes/html/');

set_include_path(
        CLASSES_GERAL . PATH_SEPARATOR .
        CLASSES_CONTROLE . PATH_SEPARATOR .
        CLASSES_MODELO . PATH_SEPARATOR .
        CLASSES_VISAO . PATH_SEPARATOR .
//                    CLASSES_HTML . PATH_SEPARATOR . 
        get_include_path()
);

function __autoload($class)
{

    require_once APPRAIZ . "includes/library/simec/funcoes.inc";
    require_once APPRAIZ . "includes/library/simec/abstract/Controller.php";
    require_once APPRAIZ . "includes/library/simec/abstract/Model.php";
    

    if (PHP_OS != "WINNT")
    { // Se "não for Windows"
        $separaDiretorio = ":";
        $include_path = get_include_path();
        $include_path_tokens = explode($separaDiretorio, $include_path);
    } else
    { // Se for Windows
        $separaDiretorio = ";c:";
        $include_path = get_include_path();
        
        $include_path = str_replace('.;', 'c:', strtolower($include_path));
        $include_path = str_replace('/', '\\', $include_path);
        
        $include_path_tokens = explode($separaDiretorio, $include_path);
        $include_path_tokens = str_replace("//", "/", $include_path_tokens);
//        $include_path_tokens[0] = explode(";", $include_path_tokens[0]);
        
        $include_path_tokens[0] = str_replace('c:', '', $include_path_tokens[0]);
        
//        ver(PHP_OS, $include_path_tokens, $include_path,d);
        
    }

    foreach ($include_path_tokens as $prefix) {
//        $file = pathinfo($prefix, PATHINFO_BASENAME); //end(explode('/',$prefix));
//        $file = ucfirst(substr($file, 0, -1));


        $file = array_pop(explode('_', $class));
//        
        $pathModule = $prefix . $file . '.php';
        if (file_exists($pathModule))
            require_once $pathModule;

        $path[0] = $prefix . $class . '.class.inc';
        $path[1] = $prefix . $class . '.php';

        foreach ($path as $thisPath) {
            if (file_exists($thisPath))
            {
                require_once $thisPath;
                return;
            }
        }
        
    }

    
}

urlAction();

/**
     * Metodo responsavel por pegar a action da controller e renderizar na tela.
     *
     * @name urlAction
     * @return void
     *
     * @throws Exception Controller não foi informada!
     * @throws Exception Action não foi informada!
     * @throws Exception Controller não existe!
     * @throws Exception Action não existe!
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 10/06/2013
     */
    function urlAction()
    {
        
        
        
        // Se tiver o nome da controller e o nome da action,
        // Significa que e uma requisicao.
        if (isset($_POST['controller']) && isset($_POST['action']))
        {
            
            
            // Encapsulando dados post
            $nameController = 'Controller_' . ucfirst($_POST['controller']);
            $nameAction = $_POST['action'] . 'Action';

            
            // Validacoes
//            if (!$nameController)
//                throw new Exception('Controller não foi informada!');
//            if (!$nameAction)
//                throw new Exception('Action não foi informada!');
//            if (!class_exists($nameController))
//                throw new Exception('Controller não existe!');
//            if (!method_exists($nameController, $nameAction))
//                throw new Exception('Action não existe!');

            // Estanciando class Controller e exibindo na tela a action.
            $controller = new $nameController;
            echo $controller->$nameAction();
            
            
            
            // -- Caso a página requisitada seja uma página existente, realiza o log de estatísticas - Verificação necessária pois
            // -- ainda não foi possível reproduzir o erro no sistema financeiro que faz com que todas as imagens do tema do sistema
            // -- sejam requeridas pelo browers como um módulo. Esta mesma verificação é feita no controleAcesso no momento de
            // -- incluir os arquivos.
            if (file_exists(realpath(APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc"))) {
                simec_gravar_estatistica();
            }
            exit;
        }
    }
