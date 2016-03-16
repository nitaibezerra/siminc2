<?php
class Model_Conceito extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'pet';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'conceito';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);
        
        $this->entity['qmeid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['conid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['texto'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'YES' , 'maximum' => '500' , 'contraint' => '');
    }
}
