<?php

class Model_UsuarioResponsabilidade extends Abstract_Model {

    protected $_schema = 'contratogestao';
    protected $_name = 'usuarioresponsabilidade';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['rpuid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => 'fk');
        $this->entity['rpustatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['rpudata_inc'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['conid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
    }

    public function salvarUsuarioRespContrato($idContrato) {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);
        $this->setAttributeValue('pflcod', $perfis[0] );
        $this->setAttributeValue('usucpf', $_SESSION['usucpf']);
        $this->setAttributeValue('rpustatus', 'A');
        $this->setAttributeValue('rpudata_inc', date('d/m/Y h:i:s'));
        $this->setAttributeValue('entid', null);
        $this->setAttributeValue('conid', $idContrato);
        $this->save();
    }
}
