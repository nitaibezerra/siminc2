<?php

class Controller_Painel extends Abstract_Controller
{
    public function __construct()
    {
        parent::__construct();
        $this->view->titulo = 'Planos Táticos Operacionais';

        $this->view->solucao = new Model_Solucao(false);
        $this->view->tema = new Model_Tema(false);
        $this->view->etapa = new Model_Etapa(false);
        $this->view->atividade = new Model_Atividade(false);

        $this->view->temaSolucao = new Model_Temasolucao(false);
        $this->view->acaoSolucao = new Model_Acaosolucao(false);
        $this->view->metaSolucao = new Model_Metasolucao(false);
        $this->view->indicadorSolucao = new Model_Indicadorsolucao(false);
        $this->view->responsavelSolucaoSe = new Model_Responsavelsolucao(false);
        $this->view->responsavelSolucaoSeAut = new Model_Responsavelsolucao(false);
    }

    public function indexAction()
    {
        $_SESSION['solid'] = null;
        $_SESSION['etpid'] = null;
        $_SESSION['atvid'] = null;
        $_SESSION['acaids'] = null;
        $solid = (int)$this->getPost('solid');
		$this->view->solid = $solid;
        $this->view->dado = $this->view->solucao->getAllSolucao($solid);
        $this->render(__CLASS__, __FUNCTION__);
    }

	public function versaoImpressaoAction()
	{
		$_SESSION['solid'] = null;
		$_SESSION['etpid'] = null;
		$_SESSION['atvid'] = null;
		$_SESSION['acaids'] = null;
		$solid = (int)$_GET['solid'];
		$this->view->solid = $solid;
		$this->view->dado = $this->view->solucao->getAllSolucao($solid);
		$this->render(__CLASS__, __FUNCTION__);
	}

    public function listarAction()
    {
        $params = array();
        parse_str($_POST['parans'], $params);

        $this->view->data = $this->view->solucao->getDadosGrid($params);
        $this->view->listing = $this->view->solucao->getListing();
        $this->render(__CLASS__, __FUNCTION__);
    }
}
