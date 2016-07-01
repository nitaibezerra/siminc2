<?php

class Controller_Questoes extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->categoria = new Model_Categoriaquestoesbinaria(false);
		$this->view->questaoBinaria = new Model_Questaobinaria(false);
		$this->view->questaoMultiplaEscolha = new Model_Questaomultiplaescolha(false);
		$this->view->eixo = new Model_Eixo(false);

		$id = $_POST['id'];
		if (!empty($id)) {
			$this->view->eixo->getEixo($id);
			$this->view->subtitulo = $this->view->eixo->getTitulo();
			$this->view->categoria->setAttributeValue('ideixo', $id);
			$this->view->questaoBinaria->setAttributeValue('ideixo', $id);
			$this->view->questaoMultiplaEscolha->setAttributeValue('ideixo', $id);

			if($this->view->eixo->getAttributeValue('tipo') == 'M' ){
				$this->view->titulo = "Cadastrar Questão Multipla Escolha";
			} elseif($this->view->eixo->getAttributeValue('tipo') == 'B' ){
				$this->view->titulo = "Controle de Categoria de Questões Binárias";
			}
		}
	}

	public function indexAction()
	{
		$id = (int)$_POST['id'];
		if (!empty($id)) {
			$this->view->eixo->getEixo($id);
		}
		$this->render(__CLASS__, __FUNCTION__);
	}
}
