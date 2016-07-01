<?php

class Model_Tema extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'tema';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['temid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label'=>'ID Tema');
        $this->entity['temdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' =>'', 'label'=>'Tema');
        $this->entity['temstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '');
    }

    public function getOptionsTema() {
        $temas = $this->getTemas();
        return $this->getOptions($temas, array(), 'temid');
    }

    public function getTemas() {
        $sql = "SELECT temid as codigo, temdsc as descricao  FROM pto.tema WHERE temstatus = 'A' ORDER BY temid ";
        return $this->_db->carregar($sql);
    }


}
