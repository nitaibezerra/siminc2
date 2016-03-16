<?php

class Controller_CategoriaBinaria extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->titulo = 'Controle de Categoria de Questões Binárias';
		$this->view->categoria = new Model_Categoriaquestoesbinaria(false);
		$this->view->eixo = new Model_Eixo(false);

		$id = (int)$_GET['id'];
		if (!empty($id)) {
			$this->view->eixo->getEixo($id);
			$this->view->subtitulo = $this->view->eixo->getTitulo();
			$this->view->categoria->setAttributeValue('ideixo', $id);
		}
	}

	public function indexAction()
	{
		$id = (int)$_POST['id'];
		if (!empty($id)) {
			$this->view->categoria->getById($id);
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function editarAction()
	{
		$this->view->titulo = 'Atualizar Questão Multipla Escolha';
		$id = (int)$_POST['id'];

		$this->view->categoria->getById($id);
		$this->view->eixo->getEixo($this->view->categoria->getAttributeValue('ideixo'));
		$this->view->subtitulo = $this->view->eixo->getTitulo();
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function excluirAction()
	{
		$model = new Model_Categoriaquestoesbinaria();
		try {
			$id = (int)$this->getPost('id');
			if( $model->excluir($id)){
				$return = array('status' => true, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'type' => 'success');
			} else {
				$return = array('status' => false, 'msg' => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO), 'type' => 'danger');
			}

		} catch (Exception $exc) {
			if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
				echo $exc->getTraceAsString();
			}
			$return = array('status' => false, 'msg' => (self::ERRO_AO_EXCLUIR), 'type' => 'danger');
		}
		$return['msg'] = '<div class="alert alert-' . $return['type'] . '" role="alert">' . $return['msg'] . '</div>';
		echo simec_json_encode($return);
	}

	public function salvarAction()
	{
		if ($this->view->categoria->salvar()) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'type' => 'danger', 'result' => $this->view->categoria->error);
		}
		echo simec_json_encode($return);
	}

	public function listarAction()
	{
		$id = (int)$_POST['ideixo'];
		if (!empty($id)) {
			$this->view->eixo->getEixo($id);
			$this->view->subtitulo = $this->view->eixo->getTitulo();
			$this->view->categoria->setAttributeValue('ideixo', $id);
		}
		$this->render(__CLASS__, __FUNCTION__);
	}
}
