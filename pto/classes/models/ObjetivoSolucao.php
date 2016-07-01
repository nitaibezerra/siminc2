<?php

class Model_ObjetivoSolucao extends Abstract_Model
{
	protected $_schema = 'pto';
	protected $_name = 'objetivosolucao';
	public $entity = array();

	public function __construct($commit = true)
	{
		parent::__construct($commit);

		$this->entity['objid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['obeid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
	}

	public function salvarObjetivoSolucao($arrayObjetivos, $idSolucao, $arrayMetas = array())
	{
		$metaSolucao = new Model_Metasolucao();
		if (is_array($arrayMetas) && (in_array(Model_Metasolucao::CORPO_LEI_ID, $arrayMetas) || $metaSolucao->metasInvalidas($arrayMetas))) {
			return true;
		}

		if (is_array($arrayObjetivos)) {
			$this->deleteAllByValues(array('solid' => $idSolucao));
			foreach ($arrayObjetivos as $objetivosID) {
				$this->setAttributeValue('solid', $idSolucao);
				$this->setAttributeValue('obeid', $objetivosID);
				$id = $this->save();
				if ($id == false) {
					throw new Exception('Erro ao inserir o Objetivo Estratégico.');
				}
			}
		} else {
			$this->error[] = array("name" => 'obeid', "msg" => ('Não pode estar vazio'));
			throw new Exception('Nenhum Objetivo Estratégico foi selecionado!');
		}
	}
}
