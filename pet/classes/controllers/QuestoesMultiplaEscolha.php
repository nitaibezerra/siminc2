<?php

class Controller_QuestoesMultiplaEscolha extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->questionario = new Model_Questionario();
		$this->view->eixo = new Model_Eixo();
		$this->view->categoria = new Model_Categoriaquestoesbinaria(false);
		$this->view->questaoMultiplaEscolha = new Model_Questaomultiplaescolha(false);
		$this->view->eixo = new Model_Eixo(false);
		$this->view->conceito = new Model_Conceito(false);

		$id = (int)$_GET['id'];
		if (!empty($id)) {
			$this->view->eixo->getEixo($id);
			$this->view->subtitulo = $this->view->eixo->getTitulo();
			$this->view->questaoMultiplaEscolha->setAttributeValue('ideixo', $id);
		}
		$this->view->titulo = "Questão Multipla Escolha";
	}

	public function indexAction()
	{
		$id = (int)$_POST['id'];
		if (!empty($id)) {
			$this->view->questaoMultiplaEscolha->getById($id);
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function editarAction()
	{
		$id = (int)$_POST['id'];
		$this->view->questaoMultiplaEscolha->populateEntity( array('qmeid'=>$id) );

		$this->view->eixo->getEixo( $this->view->questaoMultiplaEscolha->getAttributeValue('ideixo') );

		$this->view->conceitos = $this->view->conceito->getAllByValues(array('qmeid '=>$this->view->questaoMultiplaEscolha->getAttributeValue('qmeid')  ));
		$this->view->conceitos = (is_array($this->view->conceitos) ? $this->view->conceitos : array());

		$this->view->eixo->getEixo($this->view->questaoMultiplaEscolha->getAttributeValue('ideixo') );
		$this->view->subtitulo = $this->view->eixo->getTitulo();

		$this->render(__CLASS__, __FUNCTION__);
	}

	public function excluirAction()
	{
		$model = new Model_Questaomultiplaescolha();
		try {
			$id = (int)$this->getPost('id');
			if ($model->excluir($id)) {
				$return = array('status' => true, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'type' => 'success');
			} else {
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
		if ($this->view->questaoMultiplaEscolha->salvar()) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'type' => 'danger', 'result' => $this->view->questaoMultiplaEscolha->error);
		}
		echo simec_json_encode($return);
	}

	public function listarAction()
	{
		$this->view->questaoMultiplaEscolha->setAttributeValue('ideixo', $this->getPost('ideixo'));
		$this->render(__CLASS__, __FUNCTION__);
	}
}
