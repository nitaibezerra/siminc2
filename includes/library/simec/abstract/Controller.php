<?php

abstract class Abstract_Controller
{

    const DADOS_ALTERADOS_COM_SUCESSO = 'Dados alterados com sucesso!';
    const DADOS_EXCLUIDOS_COM_SUCESSO = 'Dados excluídos com sucesso!';
    const DADOS_SALVO_COM_SUCESSO = 'Dados salvo com sucesso!';
    const ERRO_AO_ALTERAR = 'Erro ao alterar dado. <br> consulte o administrador do sistema!';
    const ERRO_AO_EXCLUIR = 'Erro ao excluir dados. <br> consulte o administrador do sistema!';
    const ERRO_AO_SALVAR = 'Erro ao salvar os dados. <br> consulte o administrador do sistema!';
    const ERRO_SEM_PERMISAO = 'Você não possui permissão para executar esta funcionalidade!';
    const REGISTRO_POSSUI_VINCULO = 'Este registro não pode ser excluido, pois outros registros dependem dele';

    public $view;

    public function __construct()
    {
		$this->view = new stdClass();
    }

    public function render($controller, $action)
    {
        $controller = strtolower(str_replace('Controller_', '', $controller));
        if (strripos($action, 'Action')) {
            $action = str_replace('Action', '', $action);

            if ($this->view) {
                foreach ($this->view as $key => $value)
                    $this->{$key} = $value;
            }

            $modulo = explode('/', strtolower(trim($_SERVER['SCRIPT_NAME'])));
            $modulo = $modulo[1];

            $path = APPRAIZ . $modulo . "/classes/views/{$controller}/";
            $path = $path . $action . ".php";
            if (file_exists($path)) {
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
        $user->superuser = ($_SESSION['superuser'] == 1) ? true : false;

        return $user;
    }

    public function isPerfil($idPerfil)
    {
        global $db;

        $usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
        $sisid = !$sisid ? $_SESSION['sisid'] : $sisid;

        $sql = "select  pu.pflcod
			from
				seguranca.perfilusuario pu
			inner join
				seguranca.perfil p on p.pflcod = pu.pflcod
			and
				pu.usucpf = '$usucpf'
			and
				p.sisid = $sisid
                        and 
                            p.pflcod = {$idPerfil}
			and
				pflstatus = 'A'";

        return $db->pegaUm($sql);
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
        if ($db->testa_superuser())
            return 1;
        else {
            $sql = "select *
                        from seguranca.perfilusuario
                        where usucpf = '{$_SESSION['usucpf']}'
                        and pflcod = " . K_PERFIL_CONSULTA;
            $result = $db->pegaLinha($sql);

            if ($result)
                return 3;
        }
    }

    public function getPost($name, $default = null)
    {
        return (isset($_POST[$name])) ? $_POST[$name] : $default;
    }

    public function datesIsValid($date1, $date2)
    {
        $date1 = implode("", array_reverse(explode("/", $date1)));
        $date2 = implode("", array_reverse(explode("/", $date2)));
        return ($date1 > $date2) ? false : true;
    }

    public function dateConvert(&$date)
    {
        $date = implode("/", array_reverse(explode("-", $date)));
    }

    public function dateTimeConvert(&$date)
    {
        if (strpos($date, ' ')) {
            $date = explode(' ', $date);
            $date = implode("/", array_reverse(explode("-", $date[0]))) . ' ' . $date[1];
        } else {
            $date = implode("/", array_reverse(explode("-", $date)));
        }
    }

}
