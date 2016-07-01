<?php

class Controller_Resposta extends Abstract_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->view->resposta = new Model_Resposta(false);
	}


	public function salvarBinarioAction()
	{
		if ($this->view->resposta->salvarBinaria()) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'idform' => '#form-RespostaBinario' . $_POST['ideixo'], 'result' => $this->view->resposta->error);
		}
		echo simec_json_encode($return);
	}

	public function salvarMultiplaEscolhaAction()
	{
		if ($this->view->resposta->salvarMultiplaEscolha()) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'idform' => '#form-RespostaMultiplaEscolha' . $_POST['ideixo'], 'result' => $this->view->resposta->error);
		}
		echo simec_json_encode($return);
	}

	public function salvarConsideracaoFinalAction()
	{
		if ($this->view->resposta->salvarConsideracaoFinal()) {
			$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO), 'idGrupo'=>$_POST['idGrupo']);
		} else {
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $this->view->resposta->error );
		}
		echo simec_json_encode($return);
	}


}
