<?php

class Controller_Questionario extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->titulo = 'Cadastrar Questionário - <small>Planos Táticos Operacionais </small>';
		$this->view->questionario = new Model_Questionario(false);
		$this->view->grupo = new Model_Grupopet();
	}

	public function indexAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function editarAction()
	{
		$this->view->titulo = 'Atualizar Questionário - <small>Planos Táticos Operacionais </small>';
		$id = (int)$_POST['id'];
		$this->view->questionario->getQuestionario($id);
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function reabrirAction()
	{
		$id = (int)$_POST['id'];
		$consideracoesfinais = new Model_Consideracoesfinais();
		$consideracoesfinais->reabrirQuestionario($id);
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function excluirAction()
	{
		$questionario = new Model_Questionario();
		$id = (int)$this->getPost('id');

		if ($questionario->questionarioEmPreechimento($id)) {
			try {
				$questionario->excluir($id);
				$return = array('status' => true, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'type' => 'success');
			} catch (Exception $exc) {
				if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
					echo $exc->getTraceAsString();
				}
				$return = array('status' => false, 'msg' => (self::ERRO_AO_EXCLUIR), 'type' => 'danger');
			}
		} else {
			$return = array('status' => false, 'msg' => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO), 'type' => 'danger' );
		}
		$return['msg'] = '<div class="alert alert-' . $return['type'] . '" role="alert">' . $return['msg'] . '</div>';
		echo simec_json_encode($return);
	}

	public function salvarAction()
	{
		if ($this->view->questionario->salvar()) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $this->view->questionario->error);
		}
		echo simec_json_encode($return);
	}

	public function listarAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}
}
