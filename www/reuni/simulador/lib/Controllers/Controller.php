<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class Controller {

protected $model;
protected $view;


	protected function __construct($model,$verificausuario = TRUE){
		$this->model = $model;
		if ($verificausuario) $this->verificaUsuario();
	}

	protected function verificaUsuario() {
    	session_start();
		if (!isset($_SESSION['USUARIO_INSTITUICAO'])) {
			unset($_SESSION['USUARIO_LOGIN']);
			unset($_SESSION['USUARIO_NOME']);
			unset($_SESSION['USUARIO_INSTITUICAO']);
			session_destroy();
			//header( 'Location: index.php') ;
            header( "<script language='javascript'> alert('Acesso Negado.\nVocê precisa estar autenticado.'); window.close(); </script>");
			return false;
		}
		$this->model->instituicao = $_SESSION['USUARIO_INSTITUICAO'];
		$this->model->login =$_SESSION['USUARIO_LOGIN'];
		$this->model->nome =$_SESSION['USUARIO_NOME'];
		$this->model->accept_encoding =$_SERVER['HTTP_ACCEPT_ENCODING'];
	}

    public function getView () {
        return $this->view;
    }

}

?>
