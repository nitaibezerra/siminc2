<?php

class Controller_Eixo extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->questionario = new Model_Questionario();
		$this->view->eixo = new Model_Eixo();

		$id = (int)$_GET['id'];
		if(empty($id)){
			$id = (int)$_POST['id'];
		}
		if (!empty($id)) {
			$this->view->questionario->getQuestionario($id);
			$this->view->subtitulo = $this->view->questionario->getTitulo();
		}
		$this->view->titulo = "Cadastrar Eixo";
	}

	public function indexAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function editarAction()
	{
		$this->view->titulo = 'Atualizar Eixo';
		$id = (int)$_POST['id'];
		$this->view->eixo->getEixo($id);
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function excluirAction()
	{
		$eixo = new Model_Eixo();
		try {
			$id = (int)$this->getPost('id');

			if($eixo->excluir($id)){
				$return = array('status' => true, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'type' => 'success');
			}else {
				$return = array('status' => false, 'msg' => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO), 'type' => 'danger', 'result' => $this->view->eixo->error);
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
		if ($this->view->eixo->salvar($this->view->questionario->getAttributeValue('queid'))) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'type' => 'danger', 'result' => $this->view->eixo->error);
		}
		echo simec_json_encode($return);
	}

	public function listarAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function formAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}
}
