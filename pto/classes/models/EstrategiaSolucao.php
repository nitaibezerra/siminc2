<?php

class Model_EstrategiaSolucao extends Abstract_Model {

    protected $_schema = 'pto';
    protected $_name = 'estrategiasolucao';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['etsid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['estid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
    }

	public function salvarEstrategia($arrayEstrategia, $idSolucao, $arrayMetas){
		$metaSolucao = new Model_Metasolucao();
		if ($metaSolucao->metasInvalidas($arrayMetas)) {
			return true;
		}
		if (is_array($arrayEstrategia) ) {
			$this->deleteAllByValues(array('solid' => $idSolucao));
			foreach( $arrayEstrategia as $estrategiaID){
				$this->setAttributeValue('solid', $idSolucao);
				$this->setAttributeValue('estid', $estrategiaID);
				$id = $this->save();
				if($id == false){
					throw new Exception('Erro ao inserir Estrategia.');
				}
			}
		}
	}
}
