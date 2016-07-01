<?php

abstract class Abstract_Controller
{
    public $view;

    public function __construct()
    {
        header("Content-Type: text/html;  charset=ISO-8859-1",true);

//        $actions = get_class_methods($this);

//        foreach($actions as $action){
//            $path = APPRAIZ . "pes/classes/views/";
//            $path = $path . $action . ".php";
//
//            if(file_exists($path)){
//                require_once $path;
//            }
//        }

//        exit;
    }

    public function render($controller, $action)
    {
//        $action = __FUNCTION__;

        $controller = strtolower(str_replace('Controller_', '', $controller));
        if(strripos($action, 'Action')){
            $action = str_replace('Action', '', $action);

            if($this->view){
                foreach($this->view as $key => $value)
                    $this->{$key} = $value;
            }

            $path = APPRAIZ . "scrum/classes/views/{$controller}/";
            $path = $path . $action . ".php";
            if(file_exists($path)){
                require_once $path;
            } else {
                throw new Exception('Esta action não possui view!');
            }
        } else {
            throw new Exception('Este método não é uma action!');
        }
    }

    public function user()
    {
        $user = new stdClass();
        $user->cpf = $_SESSION['usucpf'];
        $user->nome = $_SESSION['usunome'];
        $user->email = $_SESSION['usuemail'];
        $user->superuser = ($_SESSION['superuser'] == 1)? true : false;

        return $user;
    }

    /**
     * Controle de permissao na tela pro usuario.
     *
     * @global type $db
     * @return integer | 1 - Ver / Editar / Excluir | 2 - Ver / editar | 3 - Ver
     */
    public function permission()
    {
        global $db;
        if($db->testa_superuser())
            return 1;
        else {
            $sql = "select *
                        from seguranca.perfilusuario
                        where usucpf = '{$_SESSION['usucpf']}'
                        and pflcod = " . K_PERFIL_CONSULTA;
            $result = $db->pegaLinha($sql);

            if($result)
                return 3;
        }
    }

    public function getPost($name, $default = null)
    {
        return (isset($_POST[$name]))? $_POST[$name] : $default ;
    }

    public function datesIsValid($date1, $date2)
    {
         $date1 = implode("",array_reverse(explode("/",$date1)));
         $date2 = implode("",array_reverse(explode("/",$date2)));

         return ($date1 > $date2)? false : true;
    }

    public function dateConvert(&$date)
    {
        $date = implode("/",array_reverse(explode("-",$date)));
    }
    public function dateTimeConvert(&$date)
    {
        $date = implode("/",array_reverse(explode("-",$date)));
    }
}