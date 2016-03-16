<?php

class Model_Secretaria extends Abstract_Model
{
	protected $_schema = 'painel';
	protected $_name = 'secretaria';
	public $entity = array();

	public function __construct($commit = true)
	{
		parent::__construct($commit);

		$this->entity['secid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['secdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '');
		$this->entity['secstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '');
		$this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
	}

	public function getOptionsSecretaria()
	{
		$where = '';
		if(is_array($_SESSION['secid']) and !empty($_SESSION['secid'])){
			$secid = implode( ',', $_SESSION['secid'] );
			$where = "AND secid in ({secid})";
		}
		$sql = "SELECT secid as codigo, secdsc as descricao FROM painel.secretaria WHERE secstatus = 'A'
        {$where}
        ORDER BY secdsc ";
		$dados = $this->_db->carregar($sql);
		return $this->getOptions($dados, array(), 'secid');
	}
}
