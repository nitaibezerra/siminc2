<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */
class LoginController extends Controller {

    public function __construct($dao) {
    	//parent::__construct(new LoginModel($dao),FALSE);
        parent::__construct(new LoginModel($dao),FALSE);
//    	session_start();
//		unset($_SESSION['USUARIO_LOGIN']);
		unset($_SESSION['USUARIO_NOME']);
		unset($_SESSION['USUARIO_INSTITUICAO']);
//		session_destroy();
//		if (!$dao->isConnected()) {
//		   	$this->view = new LoginView($this->model,"Banco");
//		   	return;
//		}


		$Logado = $_SESSION['usucpf'];

		if ($Logado) {
			$_SESSION['USUARIO_INSTITUICAO'] = $_SESSION['unicod'];
			if (empty($_SESSION['USUARIO_INSTITUICAO'])) {
				print "Instituiçao Inválida!";
				exit();
			}
		}
		else
		{
			print "Acesso Negado!";
			exit();
		}

        
        
        
/*    	if (isset($_POST['Entrar'])) {
		 $login = addslashes($_POST['usuario']);
		 $senha = addslashes($_POST['senha']);
         if ($this->model->verificaUsuario($login)) {
			if ($usuario = $this->model->verificaSenha($login,$senha)) {
	         	if ($this->model->getBloqueado($usuario['co_instituicao'])) {
        	 		$this->view = new LoginView($this->model,"Bloqueado",$login);
    	     	} else {
					session_start();
					$_SESSION['USUARIO_LOGIN'] = $usuario['ds_login'];
					$_SESSION['USUARIO_NOME'] = $usuario['no_usuario'];
					$_SESSION['USUARIO_INSTITUICAO'] = $usuario['co_instituicao'];
					session_write_close();
					header( 'Location: index.php?view=unidades') ;
					exit();
	         	}
			} else {
				$this->view = new LoginView($this->model,"Senha",$login);
			}
         } else {
			$this->view = new LoginView($this->model,"Usuario");
         }
    	} else {
   			$this->view = new LoginView($this->model);
    	} */

		//$_SESSION['USUARIO_NOME'] = $usuario['no_usuario'];
		header( 'Location: index.php?view=unidades') ;
		exit();

    }

}

?>
