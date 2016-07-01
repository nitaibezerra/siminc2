<?php

class Model_Comentariodocumento extends Abstract_Model {

    protected $_schema = 'workflow';
    protected $_name = 'comentariodocumento';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['cmdid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label'=>'ID');
        $this->entity['docid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label'=>'ID Documento');
        $this->entity['cmddsc'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label'=>'Descrição');
        $this->entity['cmdstatus'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label'=>'Status');
        $this->entity['cmddata'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label'=>'Data');
        $this->entity['hstid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label'=>'ID Histórico');
    }

}
