<?php

class Controller_Default extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->titulo = 'Programa De Educação Tutorial';
		$this->view->grupo = new Model_Grupopet();
		$this->view->eixo = new Model_Eixo();
		$this->view->discente = new Model_Discente();
		$this->view->identificacaoGrupo = new Model_Identificacaogrupo();
		$this->view->colegiado = new Model_Colegiado();
		$this->view->resposta = new Model_Resposta();

		$this->view->questionario = new Model_Questionario();
		$this->view->questaoBinaria = new Model_Questaobinaria();
		$this->view->questaoMultiplaEscolha = new Model_Questaomultiplaescolha();
		$this->view->conceito = new Model_Conceito();
		$this->view->consideracoesfinais = new Model_Consideracoesfinais();

		$this->view->usuarioResp = new Model_Usuarioresponsabilidade();
		$_SESSION['dadosResposabilidade'] = $this->view->usuarioResp->getListaResposabilidade();

		if (is_array($_SESSION['dadosResposabilidade'])) {
			$des = strtoupper($_SESSION['dadosResposabilidade']['nome']);
			if (!empty($des)) $this->view->titulo .= " - <small>{$des}</small>";
		}

	}

	public function indexAction()
	{
		$perfis = pegaPerfilGeral();

		if (in_array(PET_PERFIL_CLAA, $perfis)) {
			$this->render(__CLASS__, __FUNCTION__);
		} else {
			$this->painelAction();
		}
	}

	public function reabrirGrupoAction()
	{
		$this->view->idGrupo = (int)$_POST['id'];
		$this->view->identificacaoGrupo->getIdentificacaoGrupoPorIdGrupo($this->view->idGrupo);
		$this->view->questionario->getQuestionarioAtual();

		$idgid = $this->view->identificacaoGrupo->getAttributeValue('idgid');
		$queid = $this->view->questionario->getAttributeValue('queid');

		if(!empty($idgid) && !empty($queid)){
			$this->view->consideracoesfinais->getByIdQuestionarioIdGrupo( $idgid , $queid );
			$cofid = $this->view->consideracoesfinais->getAttributeValue('cofid');
			if(!empty($cofid)){
				$rtn = $this->view->consideracoesfinais->reabrirQuestionario($cofid);
				if($rtn){
					echo 'Questionario aberto com sucesso!';
				}
			}
		}
		exit;
	}

	public function selecionarGrupoAction()
	{
		$this->view->somenteLeitura = false;
		$this->view->idGrupo = (int)$_POST['id'];
		$this->view->grupoInfos = $this->view->grupo->getInformacoesBasicas($this->view->idGrupo);
		$this->view->identificacaoGrupo->getIdentificacaoGrupoPorIdGrupo($this->view->idGrupo);
		$this->view->questionario->getQuestionarioAtual();

		$idgid = $this->view->identificacaoGrupo->getAttributeValue('idgid');
		$queid = $this->view->questionario->getAttributeValue('queid');

		$_SESSION['finalizado'] = $this->view->resposta->finalizado($idgid, $queid);

		if ($this->view->identificacaoGrupo->getAttributeValue('idgid')) {
			$this->view->respostas = $this->view->resposta->getByGrupo($this->view->identificacaoGrupo->getAttributeValue('idgid'));
			$this->view->exibirAvaliacao = true;
		} else {
			$this->view->respostas = array();
			$this->view->exibirAvaliacao = false;
		}

		$this->render(__CLASS__, __FUNCTION__);
	}

	public function visualizarGrupoAction()
	{
		$perfis = pegaPerfilGeral();
		$superUser = in_array(PET_PERFIL_SUPER_USUARIO, $perfis);
		$this->view->somenteLeitura = true && (!$superUser);

		$this->view->idGrupo = (int)$_POST['id'];
		$this->view->grupoInfos = $this->view->grupo->getInformacoesBasicas($this->view->idGrupo);
		$this->view->identificacaoGrupo->getIdentificacaoGrupoPorIdGrupo($this->view->idGrupo);
		$this->view->questionario->getQuestionarioAtual();

		$idgid = $this->view->identificacaoGrupo->getAttributeValue('idgid');
		$queid = $this->view->questionario->getAttributeValue('queid');

		$_SESSION['finalizado'] = $this->view->resposta->finalizado($idgid, $queid);

		if ($this->view->identificacaoGrupo->getAttributeValue('idgid')) {
			$this->view->respostas = $this->view->resposta->getByGrupo($this->view->identificacaoGrupo->getAttributeValue('idgid'));
			$this->view->exibirAvaliacao = true;
		} else {
			$this->view->respostas = array();
			$this->view->exibirAvaliacao = false;
		}

		$this->render(__CLASS__, __FUNCTION__);
	}


	public function listaAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function painelAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function listagruposAction()
	{
		$this->render(__CLASS__, __FUNCTION__);
	}

}
