<?php

class Model_Categoriaquestoesbinaria extends Abstract_Model
{

	protected $_schema = 'pet';
	protected $_name = 'categoriaquestoesbinaria';
	public $entity = array();

	public function __construct($commit = true)
	{
		$this->questionario = new Model_Questionario();
		$this->eixo = new Model_Eixo();

		parent::__construct($commit);
		$this->entity['cqbid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['ideixo'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['nome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '150', 'contraint' => '');
		$this->entity['numerocategoria'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['cqbstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
	}

	public function getLista()
	{
		$dados = $this->getAllByValues(array('cqbstatus' => 'A', 'ideixo' => $this->getAttributeValue('ideixo') ), array('numerocategoria'));
		$dados = $this->tratarDados($dados);
		$dados = ($dados ? $dados : null);

		$listagem = new Listing(false);
		$listagem->setPerPage(30);
		$listagem->setActions(array('edit' => 'editar_categoria', 'delete' => 'apagar_categoria', 'list' => 'selecionar_categoria'));
		$listagem->setHead(array('Número Categoria', 'Nome'));
		$listagem->setEnablePagination(false);
		$listagem->setClassTable('table table-striped table-condensed table-bordered customizacao');
		$listagem->listing($dados);
	}

	public function tratarDados($dados)
	{
		if (is_array($dados)) {
			$data = array();
			foreach ($dados as $key => $questao) {
				$data[$key]['cqbid'] = $questao['cqbid'];
				$data[$key]['numerocategoria'] = $questao['numerocategoria'];
				$data[$key]['nome'] = $questao['nome'];
			}
			return $data;
		} else {
			return array();
		}

	}

	public function excluir($id)
	{
		$this->populateEntity(array('cqbid' => $id));

		$this->eixo->getEixo($this->getAttributeValue('ideixo'));
		$this->questionario->getQuestionario($this->eixo->getAttributeValue('queid'));

		if ($this->questionario->questionarioEmPreechimento()) {
			$this->setAttributeValue('cqbstatus', 'I');
			$this->setDecode(false);
			$this->treatEntityToUser();
			$this->save();
		} else {
			return false;
		}

	}

	public function salvar()
	{
		$this->populateEntity($_POST);
		$this->eixo->getEixo($this->getAttributeValue('ideixo'));
		$this->questionario->getQuestionario($this->eixo->getAttributeValue('queid'));

		if ($this->questionario->questionarioEmPreechimento()) {
			$this->setAttributeValue('cqbstatus', 'A');

			$qubid = $this->getAttributeValue('cqbid');
			if ( empty($qubid) ) {
				$dados = $this->getAllByValues(array('cqbstatus' => 'A', 'ideixo' => $this->getAttributeValue('ideixo') ) );
				$cont = count($dados);
				$this->setAttributeValue('numerocategoria', $cont + 1);
			}

			return $this->save();
		}else{
			$this->error[] = array("msg" => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO));
			return false;
		}
	}

	public function getById($cqbid)
	{
		$dados = $this->getAllByValues(array('cqbid' => $cqbid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getTitulo()
	{
		return $this->getAttributeValue('nome');
	}
}
