<?php

class Model_UsuarioSistema extends Abstract_Model {

    protected $_schema = 'seguranca';
    protected $_name = 'usuario_sistema';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => 'pk');
        $this->entity['sisid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['susdataultacesso'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['susstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['suscod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
    }

    public function setUsuarioSistema($cpf, $idSistema) {
        $this->entity['usucpf']['value'] = $cpf;
        $this->entity['sisid']['value'] = $idSistema;
        $this->entity['susstatus']['value'] = 'A';
        $this->entity['suscod']['value'] = 'A';
    }

    function salvar($cpf) {
        $dados = $this->getAllByValues(array('usucpf' =>  $cpf, 'sisid' =>  $this->getAttributeValue('sisid') ));
        if (empty($dados)) {
            return $this->insert(true, true);
        } else {
            $this->setAttributeValue('suscod', 'A');
            return $this->update();
        }
    }

	function getUsuarioSistema($cpf, $idsistema){
		$dados = $this->getAllByValues(array('usucpf' =>  $cpf, 'sisid' =>  $idsistema ));
		return $dados;
	}

}
