<?php

class Controller_QuestoesBinarias extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->questaoBinaria = new Model_Questaobinaria(false);
		$this->view->categoriaQuestoesBinaria = new Model_Categoriaquestoesbinaria();

		$id = (int)$_POST['idCategoria'];
		if (!empty($id)) {
			$this->view->categoriaQuestoesBinaria->getById($id);
			$this->view->subtitulo = $this->view->categoriaQuestoesBinaria->getTitulo();
			$this->view->questaoBinaria->setAttributeValue('cqbid', $id);
		}
		$this->view->titulo =  'Questões Binárias';
	}

	public function indexAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function editarAction()
	{
		$id = (int)$_POST['id'];
		$this->view->questaoBinaria->getById($id);
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function excluirAction()
	{
		$questaoBinaria = new Model_Questaobinaria();
		try {
			$id = (int)$this->getPost('id');
			$questaoBinaria->treatEntityToUser();
			$questaoBinaria->getById($id);
			$cqbid = $questaoBinaria->getAttributeValue('cqbid');
			$cqbid = trim( $cqbid, "'" );

			if($questaoBinaria->excluir($id)){
				$return = array('status' => true, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'type' => 'success', 'cqbid'=>$cqbid);
			}else{
				$return = array('status' => false, 'msg' => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO), 'type' => 'danger', 'cqbid'=>$cqbid );
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
		if ($this->view->questaoBinaria->salvar()) {
			$this->view->questaoBinaria->treatEntityToUser();
			$cqbid = $this->view->questaoBinaria->getAttributeValue('cqbid');
			$cqbid = trim( $cqbid, "'" );

			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO), 'type' => 'success', 'cqbid'=>$cqbid);
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $this->view->questaoBinaria->error);
		}
		echo simec_json_encode($return);
	}

	public function listarAction()
	{
		$cqbid = $_POST['cqbid'];
		$this->view->questaoBinaria->setAttributeValue('cqbid', $cqbid);
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function questaoAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}
}
