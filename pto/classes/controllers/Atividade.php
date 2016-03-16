<?php

class Controller_Atividade extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->perfilUsuario = new Model_PerfilUsuario();
		$this->view->titulo = 'Planos Táticos Operacionais';
		$this->view->atividade = new Model_Atividade(false);
		$this->view->usuario = new Model_Usuario(false);
		$this->view->responsavelSolucao = new Model_Responsavelsolucao();
		$this->view->executor = null;
		$this->view->etapa = new Model_Etapa(false);
		$this->view->solucao = new Model_Solucao(false);
		$this->view->acaoSolucao = new Model_Acaosolucao(false);
	}


	public function cadastrarAction()
	{

		$solid = (int)$this->getPost('solid');
		$atvid = (int)$this->getPost('id');
		$etpid = (int)$this->getPost('etpid');
		if (!empty($solid)) {
			$_SESSION['solid'] = $solid;
			$this->view->solucao->populateEntity(array('solid' => $solid));
			$this->view->solucao->treatEntityToUser();
			$this->view->tituloSolucao = $this->view->solucao->getTituloSolucao();
		}
		if (!empty($etpid)) {
			$_SESSION['etpid'] = $etpid;
			$this->view->etapa->populateEntity(array('etpid' => $etpid));
			$this->view->etapa->treatEntityToUser();
			$this->view->acaoSolucao->setAttributeValue('acaid', $this->view->etapa->getAttributeValue('acaid'));
			$this->view->tituloEtapa_atividade = $this->view->etapa->getTituloEtapa();
		}
		if (!empty($atvid)) {
			$_SESSION['atvid'] = $atvid;
			$this->view->atividade->populateEntity(array('atvid' => $atvid));
			$this->view->atividade->treatEntityToUser();
			$cpf = $this->view->atividade->getAttributeValue('usucpf');

			if (!empty($cpf)) {
				$result = $this->view->usuario->getUsuarioByCpf($cpf);
				if (is_array($result)) {
					$user = $result[0];
				}
				$this->view->executor = $this->view->usuario->mask($user['usucpf'], '###.###.###-##') . ' - ' . $user['usunome'];
			}
		}

		if ( !empty($_SESSION['etpid']) ) {
			$this->view->data = $this->view->atividade->getDados();
			$this->view->listingAtividade = $this->view->atividade->getListing();
			$this->render(__CLASS__, __FUNCTION__);
		} else {
			echo '<br><div class="alert alert-danger" role="alert">É necessário selecionar uma etapa!</div>';
		}


	}

	public function salvarAction()
	{
		try {
			$this->view->atividade->salvarAtividade();
			$this->view->atividade->commit();
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} catch (Exception $e) {
			$this->view->atividade->rollback();
			$error = $this->view->atividade->error;
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $error);
		}
		echo simec_json_encode($return);
	}

	public function excluirAction()
	{
		$atvid = (int)$this->getPost('id');
		$etpid = (int)$this->getPost('etpid');
		try {
			$this->view->atividade = new Model_Atividade(false);
			$this->view->atividade->inativar($atvid);
			$this->view->atividade->commit();
			$return = array('status' => false, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'result' => '', 'type' => 'success', 'etpid' => $etpid);
		} catch (Exception $e) {
			$this->view->atividade->rollback();
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $e->getMessage());
		}
		echo simec_json_encode($return);
	}

	public function listarAction($etpid = null)
	{
		if ($_SESSION['etpid']) {
			$etpid = (int)$_SESSION['etpid'];
		}
		if ($this->getPost('etpid')) {
			$etpid = (int)$this->getPost('etpid');
		}
		if ($this->getPost('id')) {
			$etpid = (int)$this->getPost('id');
		}

		$this->view->data = $this->view->atividade->getDados($etpid);
		$this->view->listingAtividade = $this->view->atividade->getListing();
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function editarAction()
	{
		$atvid = (int)$this->getPost('id');
		if (!empty($atvid)) {
			$_SESSION['atvid'] = $atvid;
			$this->view->atividade->populateEntity(array('atvid' => $atvid));
			$this->view->atividade->treatEntityToUser();

			$cpf = $this->view->atividade->getAttributeValue('usucpf');
			if (!empty($cpf)) {
				$result = $this->view->usuario->getUsuarioByCpf($cpf);
				if (is_array($result)) {
					$user = $result[0];
				}
				$this->view->executor = $this->view->usuario->mask($user['usucpf'], '###.###.###-##') . ' - ' . $user['usunome'];
			}
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function adicionarExecutorAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function ordenarAction()
	{
		$novaOrdem = $this->getPost('novaOrdem');
		$etpid = ($_SESSION['etpid'] ? $_SESSION['etpid'] : $this->getPost('etpid'));
		$novaOrdem = array_filter($novaOrdem);

		foreach ($novaOrdem as $indice => $idEtapa) {
			$ordem = $indice + 1;
			$idsArray = explode('_', $idEtapa);
			$atvid = end($idsArray);

			try {
				$this->view->atividade = new Model_Atividade(false);
				$this->view->atividade->alterarOrdem($atvid, $ordem, $etpid);
				$this->view->atividade->commit();
			} catch (Exception $e) {
				$this->view->atividade->rollback();
			}
		}
		$this->listarAction($etpid);
	}
}
