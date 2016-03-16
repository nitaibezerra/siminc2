<?php

class Controller_Boletim extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->perfilUsuario = new Model_PerfilUsuario();
		$this->view->boletim = new Model_Anexosolucao();
		$this->view->solucao = new Model_Solucao();
		$this->view->titulo = 'Planos Táticos Operacionais';
	}


	public function indexAction()
	{
		$solid = ($_SESSION['solid'] ? $_SESSION['solid'] : $this->getPost('solid'));
		if (!empty($solid) and $solid != 0) {
			$this->view->solucao->populateEntity(array('solid' => $solid));
			$this->view->solucao->treatEntityToUser();
			$this->view->tituloSolucao = $this->view->solucao->getTituloSolucao();
		}

		$this->render(__CLASS__, __FUNCTION__);
	}

	public function listarAction()
	{
		$solid = ($_SESSION['solid'] ? $_SESSION['solid'] : $this->getPost('solid'));
		if (!empty($solid) and $solid != 0) {
			$this->view->solucao->populateEntity(array('solid' => $solid));
			$this->view->solucao->treatEntityToUser();
			$this->view->tituloSolucao = $this->view->solucao->getTituloSolucao();
		}

		$this->render(__CLASS__, __FUNCTION__);
	}

	public function salvarAction()
	{
		try {
			$this->view->boletim->salvarBoletim();
			$this->view->boletim->commit();
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} catch (Exception $e) {
			$this->view->boletim->rollback();
			$error = $this->view->boletim->error;
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $error);
		}
		echo simec_json_encode($return);
	}

	public function excluirAction()
	{
		$anxid = (int)$this->getPost('id');
		$solid = (int)$this->getPost('solid');
		try {
			$this->view->boletim = new Model_Anexosolucao();
			$this->view->boletim->inativar($anxid);
			$this->view->boletim->commit();
			$return = array('status' => false, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'result' => '', 'type' => 'success', 'solid' => $solid);
		} catch (Exception $e) {
			$this->view->boletim->rollback();
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $e->getMessage());
		}
		echo simec_json_encode($return);
	}

	public function downloadAction() {
		$this->view->boletim->getArquivo((int) $_GET['arqid']);
	}
}
