<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PlanilhasPDFController extends Controller {
	private $matriz;

    public function __construct($dao) {
    	parent::__construct(new PlanilhasModel($dao, FALSE));
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
				print "Intituição Inválida!";
				exit();
			}
		}
		else
		{
			print "Acesso Negado!";
			exit();
		}

   		$this->view = new PlanilhasPDFView($this->model);
    }

	public function getMatriz() {
		$this->montaMatriz();
		return $this->matriz;
	}
}
?>
