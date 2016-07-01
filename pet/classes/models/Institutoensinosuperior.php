<?php

class Model_Institutoensinosuperior extends Abstract_Model
{

    protected $_schema = 'pet';
    protected $_name = 'institutoensinosuperior';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['iesid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['nome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
    }
}
